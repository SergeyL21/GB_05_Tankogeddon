// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsProjectile.h"

#include "ProjectileMovement.h"
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>

#include "DamageTaker.h"

// --------------------------------------------------------------------------------------
APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UProjectileMovement>(TEXT("PhysicsComponent"));
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
