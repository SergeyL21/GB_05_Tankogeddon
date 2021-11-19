// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>
#include "SavingsManager.generated.h"

class USaveGame;
class ULevelSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFromSlotAction, const FString&, SlotName);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API USavingsManager : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnGameFromSlotAction OnGameLoadedFromSlot;
	
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnGameFromSlotAction OnGameSavedToSlot;
	
	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);
	
	UFUNCTION(BlueprintCallable)
	void SaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	ULevelSaveGame* GetCurrentGameObject() const { return CurrentGameObject; }

protected:
	UPROPERTY(BlueprintReadWrite)
	ULevelSaveGame* CurrentGameObject;

protected:
	void OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
	
	void OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
