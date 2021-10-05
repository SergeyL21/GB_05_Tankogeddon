// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/Border.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

#include "RadioButtons.h"

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

    if (NewGame_StartBtn)
    {
        NewGame_StartBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame_StartBtnClicked);
    }

    if (NewGame_BackBtn)
    {
        NewGame_BackBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGame_BackBtnClicked);
    }

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnNewGameBtnClicked()
{
    if (MainScreenBorder)
    {
        MainScreenBorder->SetContentColorAndOpacity(FLinearColor{ 0.33f, 0.33f, 0.33f, 1.f });
    }

    MainCanvasPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
    NewGameCanvasPanel->SetVisibility(ESlateVisibility::Visible);

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
    if (MainScreenBorder)
    {
        MainScreenBorder->SetContentColorAndOpacity(FLinearColor{ 1.f, 1.f, 1.f, 1.f });
    }

    MainCanvasPanel->SetVisibility(ESlateVisibility::Visible);
    NewGameCanvasPanel->SetVisibility(ESlateVisibility::Hidden);

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnExitBtnClicked()
{
    auto SpecificPlayer{ GetWorld()->GetFirstPlayerController() };
    UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}

