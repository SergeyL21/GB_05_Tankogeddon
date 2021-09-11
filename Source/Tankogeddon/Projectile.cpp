// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/PrimitiveComponent.h>

#include "DamageTaker.h"
#include "Cannon.h"
#include "ActorPoolSubsystem.h"

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
}

// --------------------------------------------------------------------------------------
void AProjectile::Start() 
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	StartLocation = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	return;
}

// --------------------------------------------------------------------------------------
void AProjectile::Stop()
{
	OnDestroyedTarget.Clear();
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	if (Cast<AProjectile>(OtherActor))
	//if (OtherActor == GetInstigator())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	auto bWasTargetDestroyed{ false };
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		OtherActor->Destroy();
		bWasTargetDestroyed = true;
	}
	else if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor))
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.DamageMaker = this;
		DamageData.Instigator = GetInstigator();
		DamageTaker->TakeDamage(OUT DamageData);
		bWasTargetDestroyed = DamageData.bOutIsFatalDamage;

		if (bWasTargetDestroyed && OnDestroyedTarget.IsBound())
		{
			OnDestroyedTarget.Broadcast(OtherActor);
		}
	}
	else if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OtherComp)) 
	{
		if (PrimComp->IsSimulatingPhysics())
		{
			auto ForceVector{ GetActorForwardVector() };
			OtherComp->AddImpulseAtLocation(ForceVector * PushForce, SweepResult.ImpactPoint);
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