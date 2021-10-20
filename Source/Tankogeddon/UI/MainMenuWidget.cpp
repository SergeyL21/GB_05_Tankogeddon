// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/Border.h>
#include <Components/ComboBoxString.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

#include "RadioButtons.h"
#include "StyleSet.h"

// --------------------------------------------------------------------------------------
void UMainMenuWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    ChangeUITheme(EUITheme::Bright);
    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (NewGameBtn)
    {
        NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameBtnClicked);
    }

    if (ExitBtn) 
    {
        ExitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitBtnClicked);
    }

    if (SettingsBtn)
    {
        SettingsBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsBtnClicked);
    }

    if (NewGame_StartBtn)
    {
        NewGame_StartBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame_StartBtnClicked);
    }

    if (NewGame_BackBtn)
    {
        NewGame_BackBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame_BackBtnClicked);
    }

    if (Settings_ApplyBtn)
    {
        Settings_ApplyBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettings_ApplyBtnClicked);
    }

    if (Settings_CloseBtn)
    {
        Settings_CloseBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettings_CloseBtnClicked);
    }

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::ChangeUITheme(EUITheme InTheme)
{
    const auto RadioButtonsStyleName{ InTheme == EUITheme::Bright ?
        FName("WS_RadioButtonsBright") : FName("WS_RadioButtonsClassic") };

    if (DifficultRadioButtons)
    {
        DifficultRadioButtons->WidgetStyle =
            FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(RadioButtonsStyleName);
    }

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::SetHiddenMainScreen(bool bHidden)
{
    if (MainScreenBorder)
    {
        const auto Color{ bHidden ? FLinearColor{ 0.33f, 0.33f, 0.33f, 1.f } : FLinearColor{ 1.f, 1.f, 1.f, 1.f } };

        MainScreenBorder->SetContentColorAndOpacity(Color);
        MainCanvasPanel->SetVisibility(bHidden ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Visible);
    }
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnNewGameBtnClicked()
{
    SetHiddenMainScreen(true);
    NewGameCanvasPanel->SetVisibility(ESlateVisibility::Visible);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnSettingsBtnClicked()
{
    SetHiddenMainScreen(true);
    SettingsCanvasPanel->SetVisibility(ESlateVisibility::Visible);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnNewGame_StartBtnClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), "Level_01");
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnNewGame_BackBtnClicked()
{
    SetHiddenMainScreen(false);
    NewGameCanvasPanel->SetVisibility(ESlateVisibility::Hidden);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnSettings_ApplyBtnClicked()
{
    const auto Theme{ Settings_ThemeComboBox->GetSelectedIndex() == 0 ? EUITheme::Classic : EUITheme::Bright };
    ChangeUITheme(Theme);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnSettings_CloseBtnClicked()
{
    SetHiddenMainScreen(false);
    SettingsCanvasPanel->SetVisibility(ESlateVisibility::Hidden);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnExitBtnClicked()
{
    auto SpecificPlayer{ GetWorld()->GetFirstPlayerController() };
    UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);

    return;
}

