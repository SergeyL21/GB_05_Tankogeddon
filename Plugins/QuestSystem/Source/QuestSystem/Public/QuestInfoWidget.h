// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include "QuestInfoWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UImage* InfoImage;
	
};