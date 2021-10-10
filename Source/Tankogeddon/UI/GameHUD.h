// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>
#include "GameHUD.generated.h"

UENUM()
enum class EWidgetID : uint8
{
    None,
    MainMenu,
    PlayerStatus,
    GamePause,
    GameOver
};

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere)
    TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    EWidgetID CurrentWidgetID;

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintPure)
    UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

    UFUNCTION(BlueprintPure)
    EWidgetID GetCurrentWidgetID() const { return CurrentWidgetID; }

    UFUNCTION(BlueprintCallable)
    UUserWidget* UseWidget(EWidgetID WidgetID, int32 ZOrder = 0);

    UFUNCTION(BlueprintCallable)
    void RemoveCurrentWidgetFromViewport();

    UFUNCTION(BlueprintCallable)
    UUserWidget* AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
};
