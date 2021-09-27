// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>
#include "TankogeddonGameModeBase.generated.h"

class UPlayerTankWidget;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankogeddonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// FIXME !!! move all player methods into the TankPlayerController class !!!
	void OnGameStarted();

	//UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDied();

	void PlayerUpdateHealthBar(float CurrentHealth, float MaxHealth);
	
	void PlayerChangeCannon(const FString& CannonName);

	void PlayerUpdateAmmoBar(float CurrentAmmo, float MaxAmmo);
};

#define CURRENT_GAME_MODE (Cast<ATankogeddonGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))