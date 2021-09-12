// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Particles/ParticleSystemComponent.h>

#include "DamageTaker.h"
#include "Cannon.h"
#include "ActorPoolSubsystem.h"

// --------------------------------------------------------------------------------------
const static auto EXPLODE_TRACE_TAG{ "Explode Trace" };

// --------------------------------------------------------------------------------------
// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	auto SceneComponent{ CreateDefaultSubobject<USceneComponent>(TEXT("Root")) };
	RootComponent = SceneComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->SetHiddenInGame(true);

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
	TrailEffect->SetAutoActivate(false);
}

// --------------------------------------------------------------------------------------
void AProjectile::Start() 
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	StartLocation = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TrailEffect->ActivateSystem();
	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::Explode()
{
	if (!bHasDamageRadius)
	{
		return;
	}

	auto StartPos{ GetActorLocation() };
	auto EndPos{ StartPos + FVector{0.1f} };

	auto Shape{ FCollisionShape::MakeSphere(ExplodeRadius) };
	auto Params{ FCollisionQueryParams::DefaultQueryParam };
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = EXPLODE_TRACE_TAG;
	TArray<FHitResult> AttackHit;

	auto Rotation{ FQuat::Identity };

	bool bSweepResult = GetWorld()->SweepMultiByChannel
	(
		OUT AttackHit,
		StartPos,
		EndPos,
		Rotation,
		ECollisionChannel::ECC_Visibility,
		Shape,
		Params
	);

	GetWorld()->DebugDrawTraceTag = EXPLODE_TRACE_TAG;

	if (bSweepResult)
	{
		for (const auto& HitResult : AttackHit)
		{
			auto OtherActor{ HitResult.GetActor() };
			if (OtherActor == nullptr) continue;

			if (!CheckDamageForActor(OtherActor))
			{
				auto PrimMesh{ Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()) };
				auto ForceVector{ OtherActor->GetActorLocation() - GetActorLocation() };
				ForceVector.Normalize();
				CheckPhysicsForComponent(PrimMesh, ForceVector);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::Stop()
{
	Explode();

	OnDestroyedTarget.Clear();
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TrailEffect->DeactivateSystem();

	auto Pool{ GetWorld()->GetSubsystem<UActorPoolSubsystem>() };
	if (Pool && Pool->IsActorInPool(this))
	{
		Pool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}

	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, 
									AActor* OtherActor,
									UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex,
									bool bFromSweep, 
									const FHitResult& SweepResult)
{
	if (Cast<AProjectile>(OtherActor) /* || OtherActor == GetInstigator()*/)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else 
	{
		bool bIsFatalDamage;
		if (CheckDamageForActor(OtherActor, OUT &bIsFatalDamage))
		{
			if (bIsFatalDamage && OnDestroyedTarget.IsBound())
			{
				OnDestroyedTarget.Broadcast(OtherActor);
			}
		}
		else
		{
			auto ForceVector{ GetActorForwardVector() };
			CheckPhysicsForComponent(OtherComp, SweepResult, ForceVector);
		}
	}

	Stop();
	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::Move()
{
	auto NextPosition{ GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate };
	SetActorLocation(NextPosition);
	if (FVector::Distance(NextPosition, StartLocation) > FlyRange)
	{
		Stop();
	}
	return;
}

// --------------------------------------------------------------------------------------
bool AProjectile::CheckDamageForActor(AActor* DamageTakerActor, bool *bOutIsFatal)
{
	if (IDamageTaker* DamageTakerInterface = Cast<IDamageTaker>(DamageTakerActor))
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.DamageMaker = this;
		DamageData.Instigator = GetInstigator();
		DamageTakerInterface->TakeDamage(OUT DamageData);

		if (bOutIsFatal != nullptr)
		{
			*bOutIsFatal = DamageData.bOutIsFatalDamage;
		}

		return true;
	}

	if (bOutIsFatal != nullptr)
	{
		*bOutIsFatal = false;
	}
	return false;
}

// --------------------------------------------------------------------------------------
void AProjectile::CheckPhysicsForComponent(UPrimitiveComponent* PrimComp, const FHitResult& SweepResult, const FVector& ForceVector)
{
	if (PrimComp && PrimComp->IsSimulatingPhysics())
	{
		if (bSingleImpact)
		{
			PrimComp->AddImpulseAtLocation(ForceVector * PushForce, SweepResult.ImpactPoint);
		}
		else
		{
			PrimComp->AddForceAtLocation(ForceVector * PushForce, SweepResult.ImpactPoint);
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::CheckPhysicsForComponent(UPrimitiveComponent* PrimComp, const FVector& ForceVector)
{
	if (PrimComp && PrimComp->IsSimulatingPhysics())
	{
		if (bSingleImpact)
		{
			PrimComp->AddImpulse(ForceVector * PushForce, NAME_None, true);
		}
		else
		{
			PrimComp->AddForce(ForceVector * PushForce, NAME_None, true);
		}
	}
	return;
}
