// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCellWidget.h"
#include "InventoryDragDropOperation.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/Border.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

// --------------------------------------------------------------------------------------
bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& Item,
    const FInventoryItemInfo& ItemInfo)
{
    if (bHasItem)
    {
        return false;
    }

    SetBorderColor(ItemInfo.Type);

    if (ItemInfo.Type != EItemType::Currency && Item.Count <= 0)
    {
        Clear();
        return true;
    }

    if (ItemInfo.Type != EItemType::Currency && ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemInfo.Icon.Get());
        ItemImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
        ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        //ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    }
    
    if (CountText)
    {
        const auto String{ FString::FromInt(Item.Count) };
        CountText->SetText(FText::FromString(String));
        CountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    bHasItem = true;
    StoredItem = Item;

    return true;
}

// --------------------------------------------------------------------------------------
void UInventoryCellWidget::Clear(bool bClearImage, const FText &DefaultText)
{
    SetBorderColor(EItemType::Undefined);

    if (bClearImage && ItemImage)
    {
        ItemImage->SetBrush(FSlateBrush());
        ItemImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
        ItemImage->SetVisibility(ESlateVisibility::Hidden);
        //ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
    }

    if (CountText)
    {
        CountText->SetText(DefaultText);
        CountText->SetVisibility(ESlateVisibility::Hidden);
    }

    bHasItem = false;
    StoredItem = FInventorySlotInfo{};

    return;
}

// --------------------------------------------------------------------------------------
FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDraggable && bHasItem)
    {
        if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
        {
            return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
        }

        if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
        {
            if (StoredItem.Count > 0)
            {
                OnItemUse.Broadcast(this);
            }

            if (--StoredItem.Count <= 0)
            {
                Clear();
            }

            const auto String{ FString::FromInt(StoredItem.Count) };
            CountText->SetText(FText::FromString(String));
        }
    }
    return FReply::Handled();
}

// --------------------------------------------------------------------------------------
void UInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UInventoryDragDropOperation::StaticClass());

    if (auto InventoryDragDropOperation = Cast<UInventoryDragDropOperation>(OutOperation))
    {
        InventoryDragDropOperation->SourceCell = this;
        InventoryDragDropOperation->DefaultDragVisual = this;
    }
    else
    {
        Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    }
    return;
}

// --------------------------------------------------------------------------------------
bool UInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    auto InventoryDragDropOperation{ Cast<UInventoryDragDropOperation>(InOperation) };

    if (InventoryDragDropOperation && InventoryDragDropOperation->SourceCell != this)
    {
        OnItemDrop.Broadcast(InventoryDragDropOperation->SourceCell, this);
        return true;
    }

    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

// --------------------------------------------------------------------------------------
void UInventoryCellWidget::SetBorderColor(EItemType ItemType)
{
    if (MainBorder)
    {
        switch (ItemType)
        {
        case EItemType::Consumable:
            MainBorder->SetBrushColor(FLinearColor{ 0.f, 0.67f, 1.f });
            break;
        case EItemType::Miscellaneous:
            MainBorder->SetBrushColor(FLinearColor{ 0.f, 1.f, 0.48f });
            break;
        case EItemType::Equipment:
            MainBorder->SetBrushColor(FLinearColor{ 1.f, 0.58f, 0.f });
            break;
        default:
            MainBorder->SetBrushColor(FLinearColor{ 0.5f, 0.5f, 0.5f });
            break;
        }
    }

    return;
}

