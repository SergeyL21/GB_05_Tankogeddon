// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretAIController.h"
#include "Turret.h"

// --------------------------------------------------------------------------------------
void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	TurretPawn = Cast<ATurret>(GetPawn());
	return;
}

// --------------------------------------------------------------------------------------
void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	return;
}
