// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "QuestListEntry.generated.h"

class UTextBlock;
class UButton;
class UBorder;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestListEntry : public UUserWidget
{
	GENERATED_BODY()
    
public:
	virtual void NativeConstruct() override;
    
	void SetQuestText(const FText& Text);
	void SetIsSelected(bool IsSelected);

	FSimpleDelegate OnQuestChosen;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock * QuestText;
	UPROPERTY(meta = (BindWidgetOptional))
	UButton * SelectQuestButton;
	UPROPERTY(meta = (BindWidgetOptional))
	UBorder * SelectionBorder;

	UFUNCTION()
	void SelectQuest();
};
