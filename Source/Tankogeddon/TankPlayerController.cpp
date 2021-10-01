// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include <DrawDebugHelpers.h>

#include "Tankogeddon.h"
#include "TankPawn.h"
#include "ActorPoolSubsystem.h"

#include "UI/GameHUD.h"
#include "UI/PlayerTankWidget.h"

#define GET_HUD Cast<AGameHUD>(GetHUD())

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

	if (TankPawn)
	{
		TankPawn->SetTurretTarget(MousePos);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());

	if (auto HUD = GET_HUD)
	{
		HUD->UseWidget(EWidgetID::PlayerStatus);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::RotateRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(AxisValue);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
	{
		TankPawn->FireSpecial();
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::ChangeWeapon() 
{
	if (TankPawn)
	{
		TankPawn->ChangeWeapon();
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::Die()
{
	if (auto HUD = GET_HUD)
	{
		HUD->UseWidget(EWidgetID::GameOver);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::SetHealthWidgetValue(int32 CurrentHealth, int32 MaxHealth)
{
	if (auto HUD = GET_HUD)
	{
		if (HUD->GetCurrentWidgetID() == EWidgetID::PlayerStatus)
		{
			if (auto PlayerTankWidget = Cast<UPlayerTankWidget>(HUD->GetCurrentWidget()))
			{
				PlayerTankWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::SetCannonTextBlock(const FString& CannonName)
{
	if (auto HUD = GET_HUD)
	{
		if (HUD->GetCurrentWidgetID() == EWidgetID::PlayerStatus)
		{
			if (auto PlayerTankWidget = Cast<UPlayerTankWidget>(HUD->GetCurrentWidget()))
			{
				PlayerTankWidget->UpdateCannonInfo(CannonName);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::SetAmmoWidgetValue(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (auto HUD = GET_HUD)
	{
		if (HUD->GetCurrentWidgetID() == EWidgetID::PlayerStatus)
		{
			if (auto PlayerTankWidget = Cast<UPlayerTankWidget>(HUD->GetCurrentWidget()))
			{
				PlayerTankWidget->UpdateAmmoInfo(CurrentAmmo, MaxAmmo);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPlayerController::DumpActorPoolSubsystemStats()
{
	GetWorld()->GetSubsystem<UActorPoolSubsystem>()->DumpPoolStats();
}
