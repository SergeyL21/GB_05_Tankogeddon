// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "Cannon.h"
#include "HealthComponent.h"
#include "Saving/LevelSaveGame.h"

// --------------------------------------------------------------------------------------
// Sets default values
ATurret::ATurret() 
{

}

// --------------------------------------------------------------------------------------
void ATurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	LocalTurretMesh = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (LocalTurretMesh)
	{
		TurretMesh->SetStaticMesh(LocalTurretMesh);
	}
	LocalBodyMesh = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (LocalBodyMesh)
	{
		BodyMesh->SetStaticMesh(LocalBodyMesh);
	}

	return;
}

// --------------------------------------------------------------------------------------
int32 ATurret::GetScorePoints() const
{
	return DestructionScores;
}


