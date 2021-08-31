// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

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
void AProjectile::Start(ACannon *InOwner) 
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
	// TODO: simple fix to avoid overlapping projectiles
	if (Cast<AProjectile>(OtherActor))
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
		auto CurrentInstigator{ GetInstigator() };
		if (OtherActor != CurrentInstigator)
		{
			FDamageData DamageData;
			DamageData.DamageValue = Damage;
			DamageData.DamageMaker = this;
			DamageData.Instigator = CurrentInstigator;
			DamageTaker->TakeDamage(OUT DamageData);
			bWasTargetDestroyed = DamageData.bOutIsFatalDamage;
		}

		if (bWasTargetDestroyed && OnDestroyedTarget.IsBound())
		{
			OnDestroyedTarget.Broadcast(OtherActor);
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