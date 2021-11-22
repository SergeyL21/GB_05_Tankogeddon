// Fill out your copyright notice in the Description page of Project Settings.

#include "SavingsManager.h"
#include "LevelSaveGame.h"

#include <Kismet/GameplayStatics.h>

// --------------------------------------------------------------------------------------
void USavingsManager::Init()
{
	CurrentGameObject = Cast<ULevelSaveGame>(UGameplayStatics::CreateSaveGameObject(ULevelSaveGame::StaticClass()));

	ExistingSavedSlots.Empty();
	auto& PlatformFile {FPlatformFileManager::Get().GetPlatformFile()};
	auto FilePath {FPaths::Combine(FPaths::ProjectSavedDir(), ExistingSavedSlotsFilePath)};
	if (PlatformFile.FileExists(*FilePath))
	{
		FString ExistingSavingsArray;
		if (FFileHelper::LoadFileToString(ExistingSavingsArray, *FilePath))
		{
			ExistingSavingsArray.ParseIntoArray(ExistingSavedSlots, TEXT(","));
		}
	}
	
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
void USavingsManager::ExtLoadGame(const FString& SlotName, FString& Text)
{
	auto FilePath {FPaths::Combine(FPaths::ProjectSavedDir(), SlotName + ".sav")};

	FFileHelper::LoadFileToString(Text, *FilePath);
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
void USavingsManager::ExtSaveGame(const FString& SlotName, const FString& Text)
{
	auto FilePath {FPaths::Combine(FPaths::ProjectSavedDir(), SlotName + ".sav")};
	
	FFileHelper::SaveStringToFile(
		Text,
		*FilePath, 
		FFileHelper::EEncodingOptions::ForceUnicode,
		&IFileManager::Get(), 
		FILEWRITE_EvenIfReadOnly
	);
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
	if (!ExistingSavedSlots.Contains(SlotName))
	{
		ExistingSavedSlots.Add(SlotName);
		CacheExistingSavedSlotsInfo();
	}
	return;
}

// --------------------------------------------------------------------------------------
void USavingsManager::CacheExistingSavedSlotsInfo()
{
	auto FilePath {FPaths::Combine(FPaths::ProjectContentDir(), ExistingSavedSlotsFilePath)};

	FString ExistingSavingsArray {""};
	for (const FString& Slot : ExistingSavedSlots)
	{
		ExistingSavingsArray += Slot + ",";
	}

	FFileHelper::SaveStringToFile(
		ExistingSavingsArray,
		*FilePath, 
		FFileHelper::EEncodingOptions::ForceUnicode,
		&IFileManager::Get(), 
		FILEWRITE_EvenIfReadOnly
	);
	return;
}





