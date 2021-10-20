// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "InventoryItemInfo.h"
#include "InventoryCellWidget.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class UInventoryCellWidget;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, UInventoryCellWidget* /*DraggedFrom*/, UInventoryCellWidget* /*DroppedTo*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUse, UInventoryCellWidget* /*CellWidget*/);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UInventoryCellWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    int32 IndexInInventory{ -1 };

    UPROPERTY()
    class UBaseInventoryWidget* ParentInventoryWidget;

    FOnItemDrop OnItemDrop;
    FOnItemUse OnItemUse;

protected:
    bool bHasItem{ false };

    UPROPERTY(meta = (BindWidgetOptional))
    UImage* ItemImage;

    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* CountText;

    UPROPERTY(meta = (BindWidgetOptional))
    UBorder* MainBorder;

    UPROPERTY()
    FInventorySlotInfo StoredItem;

    UPROPERTY(EditDefaultsOnly)
    bool bIsDraggable{ true };

public:
    FORCEINLINE bool HasItem() const { return bHasItem; }

    bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo);

    void Clear(const FText& DefaultText = {});

    FORCEINLINE const FInventorySlotInfo& GetItem() const { return StoredItem; }

protected:
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
    void SetBorderColor(EItemType ItemType);
    
};
