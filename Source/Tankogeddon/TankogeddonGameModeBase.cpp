// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankogeddonGameModeBase.h"
#include "UI/PlayerTankWidget.h"
#include "UI/GameHUD.h"

#define GET_CURRENT_HUD Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::OnGameStarted()
{
	if (auto HUD = GET_CURRENT_HUD)
	{
		HUD->UseWidget(EWidgetID::PlayerStatus);
	}
}

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::OnPlayerDied()
{
	if (auto HUD = GET_CURRENT_HUD)
	{
		HUD->UseWidget(EWidgetID::GameOver);
	}
}

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::PlayerUpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (auto HUD = GET_CURRENT_HUD)
	{
		if (HUD->GetCurrentWidgetID() == EWidgetID::PlayerStatus)
		{
			if (auto PlayerTankWidget = Cast<UPlayerTankWidget>(HUD->GetCurrentWidget()))
			{
				PlayerTankWidget->UpdateHealthBar(CurrentHealth, MaxHealth);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankogeddonGameModeBase::PlayerChangeCannon(const FString& CannonName)
{
	if (auto HUD = GET_CURRENT_HUD)
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
void ATankogeddonGameModeBase::PlayerUpdateAmmoBar(float CurrentAmmo, float MaxAmmo)
{
	if (auto HUD = GET_CURRENT_HUD)
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
