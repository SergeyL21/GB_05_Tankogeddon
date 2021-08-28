// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include <DrawDebugHelpers.h>

#include "Tankogeddon.h"
#include "TankPawn.h"
#include "ActorPoolSubsystem.h"

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
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("AltFire", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ATankPlayerController::ChangeWeapon);
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MouseDirection;
	DeprojectMousePositionToWorld(OUT MousePos, OUT MouseDirection);
	const auto PawnPos{ TankPawn->GetActorLocation() };
	MousePos.Z = PawnPos.Z;
	auto Direction{ MousePos - PawnPos };
	Direction.Normalize();
	MousePos = PawnPos + Direction * DEBUG_DIRECTION_LENGTH;
	DrawDebugLine(GetWorld(), PawnPos, MousePos, FColor::Green, false, 0.1f, 0, 5.f);
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::Fire()
{
	TankPawn->Fire();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::FireSpecial()
{
	TankPawn->FireSpecial();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::ChangeWeapon() 
{
	TankPawn->ChangeWeapon();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::DumpActorPoolSubsystemStats()
{
	GetWorld()->GetSubsystem<UActorPoolSubsystem>()->DumpPoolStats();
}
