// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "SpawnableItem.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API USpawnableItem : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText ItemName;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> SpawnableClass;

    UPROPERTY(EditAnywhere)
    float ItemLocationZHint;

    UPROPERTY(EditAnywhere)
    float ItemRotationYHint;

protected:
    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ItemNameText;

    UPROPERTY()
    AActor* SpawnedActor;

    UPROPERTY()
    class ATankPlayerController* PlayerController;

public:
    virtual void NativePreConstruct() override;

    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    void OnMouseButtonUp();

};
