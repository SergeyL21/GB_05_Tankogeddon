// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "PlayerTankWidget.generated.h"


class UProgressBar;
class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UPlayerTankWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* PlayerHealthBar;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* PlayerHealthText;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient, BlueprintReadOnly)
	UWidgetAnimation* PlayerTakenDamageAnimation;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* PlayerCannonText;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* PlayerAmmoBar;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* PlayerAmmoText;

public:
    virtual void NativeConstruct() override;

	void UpdateHealthBar(float CurrentHealth, float MaxHealth, bool bAnimation = true);

	void UpdateCannonInfo(const FString& CannonName);

	void UpdateAmmoInfo(float CurrentAmmo, float MaxAmmo);
};
