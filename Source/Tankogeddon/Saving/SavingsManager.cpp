// Fill out your copyright notice in the Description page of Project Settings.

#include "SavingsManager.h"
#include "LevelSaveGame.h"

#include <Kismet/GameplayStatics.h>

// --------------------------------------------------------------------------------------
void USavingsManager::Init()
{
	CurrentGameObject = Cast<ULevelSaveGame>(UGameplayStatics::CreateSaveGameObject(ULevelSaveGame::StaticClass()));
	return;
}

// --------------------------------------------------------------------------------------
bool USavingsManager::DoesSaveGameExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

// --------------------------------------------------------------------------------------
void USavingsManager::LoadGame(const FString& SlotName)
{
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0,
		FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &USavingsManager::OnGameLoadedFromSlotHandle));
	return;
}

// --------------------------------------------------------------------------------------
void USavingsManager::SaveGame(const FString& SlotName)
{
	UGameplayStatics::AsyncSaveGameToSlot(CurrentGameObject, SlotName, 0,
		FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USavingsManager::OnGameSavedToSlotHandle));
	return;
}

// --------------------------------------------------------------------------------------
void USavingsManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentGameObject = Cast<ULevelSaveGame>(SaveGame);
	OnGameLoadedFromSlot.Broadcast(SlotName);
	
	return;
}

// --------------------------------------------------------------------------------------
void USavingsManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	OnGameSavedToSlot.Broadcast(SlotName);
}





