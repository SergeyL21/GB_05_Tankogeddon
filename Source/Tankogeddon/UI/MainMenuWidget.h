// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* NewGameBtn;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
    UButton* LoadGameBtn;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
    UButton* SettingsBtn;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
    UButton* ExitBtn;

protected:
    UFUNCTION()
    void OnNewGameBtnClicked();

    UFUNCTION()
    void OnExitBtnClicked();

public:
    virtual void NativeConstruct() override;
};
