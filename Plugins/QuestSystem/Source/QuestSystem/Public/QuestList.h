// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "QuestList.generated.h"

class AQuest;
class UQuestListEntry;
class UQuestDescription;
class UScrollBox;
class UQuestListComponent;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestList : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UQuestListComponent* QuestList);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UScrollBox * StoryQuestsList;
	UPROPERTY(meta = (BindWidgetOptional))
	UScrollBox * SideQuestsList;
	UPROPERTY(meta = (BindWidgetOptional))
	UQuestDescription * ActiveQuestDescription;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestListEntry> QuestEntryClass;    
	UPROPERTY()
	TMap<AQuest*, UQuestListEntry*> QuestEntries;

	void OnActiveQuestChanged(AQuest* ActiveQuest);
};
