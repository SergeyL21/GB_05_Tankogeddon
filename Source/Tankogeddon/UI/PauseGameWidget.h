// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "PauseGameWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UPauseGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* LoadBtn;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* SaveBtn;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* ReturnBtn;

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnLoadBtnClicked();

	UFUNCTION()
	void OnSaveBtnClicked();

	UFUNCTION()
	void OnReturnBtnClicked();
};
