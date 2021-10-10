// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

// --------------------------------------------------------------------------------------
void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    /*if (GetWorld())
    {
        auto PlayerController{ GetWorld()->GetFirstPlayerController() };
        if (PlayerController)
        {
            UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, nullptr, EMouseLockMode::DoNotLock, false);
            PlayerController->bShowMouseCursor = true;
        }
    }*/

    return;
}

// --------------------------------------------------------------------------------------
UUserWidget* AGameHUD::UseWidget(EWidgetID WidgetID, int32 ZOrder)
{
    if (CurrentWidgetID == WidgetID) return CurrentWidget;

    TSubclassOf<UUserWidget> WidgetClassToUse{ WidgetClasses.FindRef(WidgetID) };
    if (WidgetClassToUse.Get())
    {
        auto ResultWidget{ AddWidgetToViewportByClass(WidgetClassToUse, ZOrder) };
        CurrentWidgetID = WidgetID;
        return ResultWidget;
    };

    return nullptr;
}

// --------------------------------------------------------------------------------------
void AGameHUD::RemoveCurrentWidgetFromViewport()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;

        CurrentWidgetID = EWidgetID::None;
    }

    return;
}

// --------------------------------------------------------------------------------------
UUserWidget* AGameHUD::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder /*= 0*/)
{
    RemoveCurrentWidgetFromViewport();

    CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport(ZOrder);
    }

    return CurrentWidget;
}


