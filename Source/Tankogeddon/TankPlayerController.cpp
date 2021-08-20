// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tankogeddon.h"
#include "TankPawn.h"

#include <DrawDebugHelpers.h>

// --------------------------------------------------------------------------------------
constexpr auto DEBUG_DIRECTION_LENGTH{ 1000.f };

// --------------------------------------------------------------------------------------
ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector mouseDirection;
	DeprojectMousePositionToWorld(OUT MousePos, OUT mouseDirection);
	const auto pawnPos{ TankPawn->GetActorLocation() };
	MousePos.Z = pawnPos.Z;
	auto direction{ MousePos - pawnPos };
	direction.Normalize();
	MousePos = pawnPos + direction * DEBUG_DIRECTION_LENGTH;
	DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Red, false, 0.1f, 0, 5.f);
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}
