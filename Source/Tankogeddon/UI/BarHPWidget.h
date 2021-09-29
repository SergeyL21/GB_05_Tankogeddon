// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "BarHPWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UBarHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* MainBar;

public:
	void SetHP(float CurrentHealth, float MaxHealth);
};
