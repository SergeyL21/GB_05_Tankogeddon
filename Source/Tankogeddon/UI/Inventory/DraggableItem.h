// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "DraggableItem.generated.h"

class UDragDropOperation;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UDraggableItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
    FText ItemName;

    UPROPERTY(EditAnywhere)
    FText ItemDescription;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UWidget> ToolTipClass;

protected:
    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* ItemNameText;

    UPROPERTY(meta = (BindWidget))
    UBorder* MainBorder;

public:
    virtual void NativePreConstruct() override;

    virtual void NativeConstruct() override;

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    
    virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
    
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

    UFUNCTION()
    void OnDragCancelledHandle(UDragDropOperation* Operation);
};
