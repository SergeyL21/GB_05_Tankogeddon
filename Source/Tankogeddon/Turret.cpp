// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "Cannon.h"
#include "HealthComponent.h"

// --------------------------------------------------------------------------------------
// Sets default values
ATurret::ATurret()
{
	auto TurretMeshTemp{ LoadObject<UStaticMesh>(this, *TurretMeshPath) };
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	auto BodyMeshTemp{ LoadObject<UStaticMesh>(this, *BodyMeshPath) };
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}
}

// --------------------------------------------------------------------------------------
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	return;
}

// --------------------------------------------------------------------------------------
int32 ATurret::GetScorePoints() const
{
	return DestructionScores;
}


