// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include <Components/Button.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

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

    return;
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnNewGameBtnClicked()
{
    UGameplayStatics::OpenLevel(GetWorld(), "Level_01");
}

// --------------------------------------------------------------------------------------
void UMainMenuWidget::OnExitBtnClicked()
{
    auto SpecificPlayer{ GetWorld()->GetFirstPlayerController() };
    UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}

