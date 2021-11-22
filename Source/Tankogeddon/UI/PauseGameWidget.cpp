// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseGameWidget.h"

#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>
#include <Components/EditableText.h>
#include <GameFramework/GameModeBase.h>

#include "Quest.h"
#include "QuestListComponent.h"
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

	const auto SlotName {SlotNameTextBox->Text.IsEmpty() ?
		GameInstance->GetQuickSaveSlotName() : SlotNameTextBox->Text.ToString()};
	//GameInstance->SaveManager->LoadGame(SlotName);
	FString Text;
	GameInstance->SaveManager->ExtLoadGame(SlotName, Text);
	LoadData->DeserializeFromJSON(Text);
	
	// destroy all base pawn actors
	for (const auto Actor : FoundActors)
	{
		auto Pawn = Cast<ABasePawn>(Actor);
		if (!Pawn->IsPlayerPawn())
		{
			//Pawn->Destroy();
		}
		else
		{
			Pawn->LoadPawnState(LoadData->Player);
			auto QuestListCmp {Cast<UQuestListComponent>(Pawn->GetComponentByClass(UQuestListComponent::StaticClass()))};
			if (QuestListCmp)
			{
				QuestListCmp->SetActiveQuest(LoadData->ActiveQuest);
			}
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);
	auto SaveData {GameInstance->SaveManager->GetCurrentGameObject()};

	if (!SaveData)
	{
		return;
	}
	
	SaveData->Enemies.Empty();
	SaveData->Quests.Empty();
	for (const auto Actor : FoundActors)
	{
		if (auto Quest = Cast<AQuest>(Actor))
		{
			SaveData->Quests.AddUnique(Quest);
		}
		else if (auto Pawn = Cast<ABasePawn>(Actor))
		{
			if (Pawn->IsPlayerPawn())
			{
				Pawn->SavePawnState(SaveData->Player);

				auto QuestListCmp {Cast<UQuestListComponent>(Pawn->GetComponentByClass(UQuestListComponent::StaticClass()))};
				if (QuestListCmp)
				{
					SaveData->ActiveQuest = QuestListCmp->GetActiveQuest();
				}
			}
			else
			{
				auto EnemySaveData {NewObject<UPawnSaveGame>()};
				Pawn->SavePawnState(EnemySaveData);
				SaveData->Enemies.Add(EnemySaveData);
			}
		}
	}
	
	const auto SlotName {SlotNameTextBox->Text.IsEmpty() ?
		GameInstance->GetQuickSaveSlotName() : SlotNameTextBox->Text.ToString()};
	//GameInstance->SaveManager->SaveGame(SlotName);
	GameInstance->SaveManager->ExtSaveGame(SlotName, SaveData->SerializeToJSON());
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



