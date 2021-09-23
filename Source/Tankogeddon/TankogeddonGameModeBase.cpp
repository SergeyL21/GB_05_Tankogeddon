// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankogeddonGameModeBase.h"
#include "UI/PlayerTankWidget.h"

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::PlayerUpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (PlayerTankWidget)
	{
		PlayerTankWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::PlayerChangeCannon(const FString& CannonName)
{
	if (PlayerTankWidget)
	{
		PlayerTankWidget->UpdateCannonInfo(CannonName);
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::PlayerUpdateAmmoBar(float CurrentAmmo, float MaxAmmo)
{
	if (PlayerTankWidget)
	{
		PlayerTankWidget->UpdateAmmoInfo(CurrentAmmo, MaxAmmo);
	}

	return;
}
