// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsProjectile.h"

#include "ProjectileMovement.h"
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>

#include "DamageTaker.h"

// --------------------------------------------------------------------------------------
APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UProjectileMovement>(TEXT("PhysicsComponent"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
}

// --------------------------------------------------------------------------------------
void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep);
	if (bShowTrajectory)
	{
		for (const auto& Position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Purple, true, 1, 0, 2);
		}
	}

	TrajectoryPointIndex = 0;
	Super::Start();
	return;
}

// --------------------------------------------------------------------------------------
void APhysicsProjectile::Explode()
{
	auto StartPos{ GetActorLocation() };
	auto EndPos{ StartPos + FVector{0.1f} };

	auto Shape{ FCollisionShape::MakeSphere(ExplodeRadius) };
	auto Params{ FCollisionQueryParams::DefaultQueryParam };
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";
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

	GetWorld()->DebugDrawTraceTag = "Explode Trace";
	
	if (bSweepResult)
	{
		for (const auto& HitResult : AttackHit)
		{
			auto OtherActor{ HitResult.GetActor() };
			if (OtherActor == nullptr) continue;

			if (auto DamageTakerActor = Cast<IDamageTaker>(OtherActor))
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.Instigator = GetOwner();
				DamageData.DamageMaker = this;

				DamageTakerActor->TakeDamage(OUT DamageData);
			}
			else if (auto PrimMesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()))
			{
				if (PrimMesh->IsSimulatingPhysics())
				{
					auto ForceVector{ OtherActor->GetActorLocation() - GetActorLocation() };
					ForceVector.Normalize();
					PrimMesh->AddImpulse(ForceVector * PushForce, NAME_None, true);
				}
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void APhysicsProjectile::Move()
{
	auto CurrentMoveVector{ CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation() };
	CurrentMoveVector.Normalize();
	auto NewLocation{ GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate };
	SetActorLocation(NewLocation);
	if (FVector::Distance(NewLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MoveAccurency)
	{
		if (++TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			Explode();
			Stop();
		}
		else
		{
			auto NewRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]) };
			SetActorRotation(NewRotation);
		}
	}
	
	return;
}
