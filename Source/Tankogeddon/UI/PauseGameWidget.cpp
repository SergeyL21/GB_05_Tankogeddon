// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseGameWidget.h"

#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>
#include <GameFramework/GameModeBase.h>

#include "Tankogeddon/BasePawn.h"
#include "Tankogeddon/TankPlayerController.h"
#include "Tankogeddon/TankogeddonGameInstance.h"
#include "Tankogeddon/Saving/SavingsManager.h"
#include "Tankogeddon/Saving/LevelSaveGame.h"

// ------------------------------------------------------------------------------------------------
void UPauseGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LoadBtn)
	{
		LoadBtn->OnClicked.AddDynamic(this, &ThisClass::OnLoadBtnClicked);
	}

	if (SaveBtn)
	{
		SaveBtn->OnClicked.AddDynamic(this, &ThisClass::OnSaveBtnClicked);
	}

	if (ReturnBtn)
	{
		ReturnBtn->OnClicked.AddDynamic(this, &ThisClass::OnReturnBtnClicked);
	}
	return;
}

// ------------------------------------------------------------------------------------------------
void UPauseGameWidget::OnLoadBtnClicked()
{
	auto GameInstance {Cast<UTankogeddonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))};
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePawn::StaticClass(), FoundActors);
	auto LoadData {GameInstance->SaveManager->GetCurrentGameObject()};

	if (!LoadData)
	{
		return;
	}

	GameInstance->SaveManager->LoadGame(GameInstance->GetQuickSaveSlotName());
	// destroy all base pawn actors
	for (const auto Actor : FoundActors)
	{
		auto Pawn = Cast<ABasePawn>(Actor);
		if (!Pawn->IsPlayerPawn())
		{
			Pawn->Destroy();
		}
		else
		{
			Pawn->LoadPawnState(LoadData->Player);
		}
	}

	// and recreate it's with old states
	auto GameMode {UGameplayStatics::GetGameMode(GetWorld())};
	for (auto LoadPawn : LoadData->Enemies)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		auto OtherPawn {GetWorld()->SpawnActor<ABasePawn>(
			LoadPawn->BasePawnClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams)
		};
			
		/*auto OtherPawn {GetWorld()->SpawnActorDeferred<ABasePawn>(
			LoadPawn->BasePawnClass,
			LoadPawn->Transform,
			GameMode,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn)
		};*/
		if (OtherPawn)
		{
			OtherPawn->LoadPawnState(LoadPawn);
		}
	}
	return;
}

// ------------------------------------------------------------------------------------------------
void UPauseGameWidget::OnSaveBtnClicked()
{
	auto GameInstance {Cast<UTankogeddonGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))};
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePawn::StaticClass(), FoundActors);
	auto SaveData {GameInstance->SaveManager->GetCurrentGameObject()};

	if (!SaveData)
	{
		return;
	}

	SaveData->Enemies.Empty();
	for (const auto Actor : FoundActors)
	{
		auto Pawn = Cast<ABasePawn>(Actor);
		if (Pawn->IsPlayerPawn())
		{
			Pawn->SavePawnState(SaveData->Player);
		}
		else
		{
			auto EnemySaveData {NewObject<UPawnSaveGame>()};
			Pawn->SavePawnState(EnemySaveData);
			SaveData->Enemies.Add(EnemySaveData);
		}
	}
	
	GameInstance->SaveManager->SaveGame(GameInstance->GetQuickSaveSlotName());
	return;
}

// ------------------------------------------------------------------------------------------------
void UPauseGameWidget::OnReturnBtnClicked()
{
	auto PlayerController {Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))};
	if (PlayerController )
	{
		PlayerController->PauseMenuEnabled(false);
	}
	return;
}



