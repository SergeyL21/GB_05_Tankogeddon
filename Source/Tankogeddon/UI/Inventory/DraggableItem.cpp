// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggableItem.h"
#include "ItemDragDropOperation.h"
#include "TooltipWidget.h"
#include <Components/TextBlock.h>
#include <Components/GridPanel.h>
#include <Components/GridSlot.h>
#include <Components/Border.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

// --------------------------------------------------------------------------------------
void UDraggableItem::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ItemNameText)
    {
        ItemNameText->SetText(ItemName);
    }

    return;
}

// --------------------------------------------------------------------------------------
void UDraggableItem::NativeConstruct()
{
    if (MainBorder && ToolTipClass)
    {
        if (auto Widget = CreateWidget<UTooltipWidget>(MainBorder, ToolTipClass.Get()))
        {
            MainBorder->SetToolTip(Widget);
            if (Widget->TooltipTextBlock)
            {
                Widget->TooltipTextBlock->SetText(ItemDescription);
            }
        }
    }

    return;
}

// --------------------------------------------------------------------------------------
FReply UDraggableItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    {
        return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
    }
    return FReply::Handled();
}

// --------------------------------------------------------------------------------------
void UDraggableItem::NativeOnDragDetected(
    const FGeometry& InGeometry, 
    const FPointerEvent& InMouseEvent, 
    UDragDropOperation*& OutOperation)
{
    auto ItemDragDropOperation{ Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass())) };

    if (ItemDragDropOperation)
    {
        ItemDragDropOperation->ItemName = ItemName;
        ItemDragDropOperation->DefaultDragVisual = this;

        SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.7f));
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        ItemDragDropOperation->OnDragCancelled.AddDynamic(this, &UDraggableItem::OnDragCancelledHandle);

        OutOperation = ItemDragDropOperation;
    }
    else
    {
        Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    }

    return;
}

// --------------------------------------------------------------------------------------
bool UDraggableItem::NativeOnDragOver(
    const FGeometry& InGeometry, 
    const FDragDropEvent& InDragDropEvent, 
    UDragDropOperation* InOperation)
{
    auto GridPanel { Cast<UGridPanel>(GetParent()) };
    auto ItemDragDropOperation { Cast<UItemDragDropOperation>(InOperation) };
    if (GridPanel && ItemDragDropOperation && this != ItemDragDropOperation->DefaultDragVisual)
    {
        int32 IndexDragged{ GridPanel->GetChildIndex(ItemDragDropOperation->DefaultDragVisual) };
        if (IndexDragged != -1)
        {
            auto GridPanelSlots{ GridPanel->GetSlots() };
            auto GridDraggedSlot{ Cast<UGridSlot>(GridPanelSlots[IndexDragged]) };

            for (auto GridPanelSlot : GridPanelSlots)
            {
                auto GridChildSlot{ Cast<UGridSlot>(GridPanelSlot) };
                auto GridChild{ GridChildSlot->Content };
                if (GridChild == this)
                {
                    GridPanel->AddChildToGrid(ItemDragDropOperation->DefaultDragVisual, GridChildSlot->Row, GridChildSlot->Column);
                    GridPanel->AddChildToGrid(GridChild, GridDraggedSlot->Row, GridDraggedSlot->Column);
                    break;
                }
            }
        }
    }
    return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

// --------------------------------------------------------------------------------------
bool UDraggableItem::NativeOnDrop(
    const FGeometry& InGeometry, 
    const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    if (InOperation && InOperation->DefaultDragVisual)
    {
        Cast<UUserWidget>(InOperation->DefaultDragVisual)->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
        InOperation->DefaultDragVisual->SetVisibility(ESlateVisibility::Visible);
    }
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

// --------------------------------------------------------------------------------------
void UDraggableItem::OnDragCancelledHandle(UDragDropOperation* Operation)
{
    SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    SetVisibility(ESlateVisibility::Visible);

    return;
}
