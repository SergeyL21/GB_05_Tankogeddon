// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretAIController.h"
#include "Turret.h"

// --------------------------------------------------------------------------------------
void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	Turret = Cast<ATurret>(BasePawn);
	return;
}

// --------------------------------------------------------------------------------------
void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Targeting();
	return;
}
