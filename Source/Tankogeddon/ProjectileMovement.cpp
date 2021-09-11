// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileMovement.h"

// --------------------------------------------------------------------------------------
TArray<FVector> UProjectileMovement::GenerateTrajectory(const FVector& StartPos, const FVector& StartVelocity, float MaxTime, float TimeStep, float MinZValue /* = 0.f*/)
{
	TArray<FVector> Trajectory;
	const FVector GravityVector{ 0, 0, Gravity };
	for (float Time = 0.f; Time < MaxTime; Time += TimeStep) 
	{
		auto Position{ StartPos + StartVelocity * Time + GravityVector * Time * Time / 2.f };
		if (Position.Z <= MinZValue)
		{
			break;
		}
		Trajectory.Add(Position);
	}
	return Trajectory;
}
