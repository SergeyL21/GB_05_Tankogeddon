// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseInventoryWidget.h"

#include "InventoryComponent.h"

// --------------------------------------------------------------------------------------
void UBaseInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    for (auto CellWidget : CellWidgets)
    {
        InitCellWidget(CellWidget);
    }

    return;
}

// --------------------------------------------------------------------------------------
UInventoryCellWidget* UBaseInventoryWidget::CreateCellWidget()
{
    if (CellWidgetClass)
    {
        const auto CellWidget{ CreateWidget<UInventoryCellWidget>(this, CellWidgetClass) };
        CellWidgets.Add(CellWidget);
        InitCellWidget(CellWidget);

        return CellWidget;
    }
    return nullptr;
}

// --------------------------------------------------------------------------------------
void UBaseInventoryWidget::InitCellWidget(UInventoryCellWidget* Widget)
{
    if (Widget)
    {
        Widget->OnItemDrop.RemoveAll(this);
        Widget->OnItemUse.RemoveAll(this);
        
        Widget->OnItemDrop.AddUObject(this, &UBaseInventoryWidget::OnItemDropped);
        Widget->OnItemUse.AddUObject(this, &UBaseInventoryWidget::OnItemUsedFunc);

        if (!Widget->ParentInventoryWidget)
        {
            Widget->ParentInventoryWidget = this;
        }
    }

    return;
}

// --------------------------------------------------------------------------------------
void UBaseInventoryWidget::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    OnItemDrop.Broadcast(DraggedFrom, DroppedTo);
}

// --------------------------------------------------------------------------------------
void UBaseInventoryWidget::OnItemUsedFunc(UInventoryCellWidget* CellWidget)
{
    if (CellWidget)
    {
        OnItemUsed.Broadcast(CellWidget);
    }

    return;
}

