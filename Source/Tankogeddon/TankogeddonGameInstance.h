// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/GameInstance.h>
#include "TankogeddonGameInstance.generated.h"

class USavingsManager;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UTankogeddonGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	USavingsManager * SaveManager;
	
public:
	virtual void Init() override;

	static FString GetQuickSaveSlotName() { return TEXT("QuickSlot"); }
};
