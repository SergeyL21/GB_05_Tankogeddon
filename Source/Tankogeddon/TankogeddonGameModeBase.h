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
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnPlayerDied();
};

#define CURRENT_GAME_MODE (Cast<ATankogeddonGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))