// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/SaveGame.h>

#include "InventoryItemInfo.h"
#include "LevelSaveGame.generated.h"

class ABasePawn;

UCLASS()
class TANKOGEDDON_API UPawnSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABasePawn> BasePawnClass;
	
	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth {0.f};

	UPROPERTY(BlueprintReadWrite)
	float MaxHealth {0.f};

	UPROPERTY(BlueprintReadWrite)
	float CurrentAmmo {0.f};

	UPROPERTY(BlueprintReadWrite)
	float MaxAmmo {0.f};
	
	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FInventorySlotInfo> InventoryItems;
};

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ULevelSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	ULevelSaveGame();
	
	UPROPERTY(BlueprintReadWrite)
	UPawnSaveGame* Player;

	UPROPERTY(BlueprintReadWrite)
	TArray<UPawnSaveGame*> Enemies;
};
