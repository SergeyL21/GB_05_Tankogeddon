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

    if (ItemImage)
    {
        ItemImage->SetBrushFromTexture(ItemInfo.Icon.Get());
        ItemImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
        //ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    }

    if (CountText)
    {
        auto String{ FString::FromInt(Item.Count) };
        CountText->SetText(FText::FromString(String));
    }

    bHasItem = true;
    StoredItem = Item;

    return true;
}

// --------------------------------------------------------------------------------------
void UInventoryCellWidget::Clear(const FText &DefaultText)
{
    SetBorderColor(EItemType::Undefined);

    if (ItemImage)
    {
        ItemImage->SetBrush(FSlateBrush());
        ItemImage->SetBrushTintColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
        //ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
    }

    if (CountText)
    {
        CountText->SetText(DefaultText);
    }

    bHasItem = false;
    StoredItem = FInventorySlotInfo{};

    return;
}

// --------------------------------------------------------------------------------------
FReply UInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDraggable && bHasItem && InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
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
