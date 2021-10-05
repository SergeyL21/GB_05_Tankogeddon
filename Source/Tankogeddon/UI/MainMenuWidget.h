// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MainMenuWidget.generated.h"

class UButton;
class URadioButtons;
class UBorder;
class UCanvasPanel;
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

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    URadioButtons* DifficultRadioButtons;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UCanvasPanel* MainCanvasPanel;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UCanvasPanel* NewGameCanvasPanel;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
    UBorder* MainScreenBorder;

    // TODO: move into the another class (NewGameWidget?)
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* NewGame_BackBtn;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* NewGame_StartBtn;

protected:
    UFUNCTION()
    void OnNewGameBtnClicked();

    UFUNCTION()
    void OnNewGame_StartBtnClicked();

    UFUNCTION()
    void OnNewGame_BackBtnClicked();

    UFUNCTION()
    void OnExitBtnClicked();

public:
    virtual void NativeConstruct() override;
};
