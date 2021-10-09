// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "MainMenuWidget.generated.h"

class UButton;
class URadioButtons;
class UBorder;
class UCanvasPanel;
class UComboBoxString;

UENUM()
enum class EUITheme {
    Classic,
    Bright
};

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

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UCanvasPanel* SettingsCanvasPanel;

    UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
    UBorder* MainScreenBorder;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UComboBoxString* Settings_ThemeComboBox;

    // TODO: move into the another class (NewGameWidget?)
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* NewGame_BackBtn;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* NewGame_StartBtn;

    // TODO: move into the another class (SettingsGameWidget?)
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* Settings_CloseBtn;

    UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
    UButton* Settings_ApplyBtn;

protected:
    UFUNCTION()
    void OnNewGameBtnClicked();

    UFUNCTION()
    void OnSettingsBtnClicked();

    UFUNCTION()
    void OnNewGame_StartBtnClicked();

    UFUNCTION()
    void OnNewGame_BackBtnClicked();

    UFUNCTION()
    void OnSettings_ApplyBtnClicked();

    UFUNCTION()
    void OnSettings_CloseBtnClicked();

    UFUNCTION()
    void OnExitBtnClicked();

public:
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

    UFUNCTION()
    void ChangeUITheme(EUITheme InTheme);

private:
    void SetHiddenMainScreen(bool bHidden);

};
