// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include <Components/UniformGridPanel.h>
#include <Components/CheckBox.h>

// --------------------------------------------------------------------------------------
void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    AllFilterToggle->OnCheckStateChanged.AddDynamic(this, &UInventoryWidget::OnAllFilterButtonToggled);
    EquipmentFilterToggle->OnCheckStateChanged.AddDynamic(this, &UInventoryWidget::OnEquipmentFilterButtonToggled);
    ConsumableFilterToggle->OnCheckStateChanged.AddDynamic(this, &UInventoryWidget::OnConsumableFilterButtonToggled);
    MiscellaneousFilterToggle->OnCheckStateChanged.AddDynamic(this, &UInventoryWidget::OnMiscellaneousFilterButtonToggled);

    return;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::Init(int32 ItemsNum)
{
    if (ItemCellsGrid)
    {
        ItemCellsGrid->ClearChildren();
        for (int32 i{ 0 }; i < ItemsNum; ++i)
        {
            auto CellWidget{ CreateCellWidget() };
            CellWidget->IndexInInventory = i;
            ItemCellsGrid->AddChildToUniformGrid(CellWidget, i / ItemsInRow, i % ItemsInRow);
        }
    }
    return;
}

// --------------------------------------------------------------------------------------
bool UInventoryWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition)
{
    if (ItemInfo.Type == EItemType::Currency)
    {
        if (GoldCell)
        {
            return GoldCell->AddItem(Item, ItemInfo);
        }
        return false;
    }

    if (ItemCellsGrid)
    {
        UInventoryCellWidget* WidgetToAddItem = nullptr;
        UInventoryCellWidget** WidgetToAddItemPtr =
            CellWidgets.FindByPredicate([SlotPosition](UInventoryCellWidget* Widget)
            {
                return Widget && Widget->IndexInInventory == SlotPosition;
            });

        if (WidgetToAddItemPtr)
        {
            WidgetToAddItem = *WidgetToAddItemPtr;
        }
        else
        {
            for (auto CellWidget : CellWidgets)
            {
                if (!CellWidget->HasItem())
                {
                    WidgetToAddItem = CellWidget;
                    break;
                }
            }
        }

        if (WidgetToAddItem)
        {
            return WidgetToAddItem->AddItem(Item, ItemInfo);
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::ClearItem(int32 SlotPosition)
{
    if (SlotPosition == -1 && GoldCell)
    {
        GoldCell->Clear(FText::FromString("0"));
    }
    else if (ItemCellsGrid)
    {
        UInventoryCellWidget** WidgetItemPtr =
            CellWidgets.FindByPredicate([SlotPosition](UInventoryCellWidget* Widget)
            {
                return Widget && Widget->IndexInInventory == SlotPosition;
            });

        if (WidgetItemPtr)
        {
            (*WidgetItemPtr)->Clear();
        }
    }
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::ClearItems()
{
    if (GoldCell)
    {
        GoldCell->Clear(FText::FromString("0"));
    }

    for (auto CellWidget : CellWidgets)
    {
        CellWidget->Clear();
    }

    return;
}

// --------------------------------------------------------------------------------------
UInventoryCellWidget* UInventoryWidget::CreateCellWidget()
{
    if (CellWidgetClass)
    {
        auto CellWidget{ CreateWidget<UInventoryCellWidget>(this, CellWidgetClass) };
        CellWidgets.Add(CellWidget);
        CellWidget->OnItemDrop.AddUObject(this, &UInventoryWidget::OnItemDropped);

        return CellWidget;
    }
    return nullptr;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    OnItemDrop.Broadcast(DraggedFrom, DroppedTo);
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::OnAllFilterButtonToggled(bool bIsChecked)
{
    if (bIsChecked)
    {
        ConsumableFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        MiscellaneousFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        EquipmentFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);

        OnItemFilterChanged.Broadcast(EItemFilter::None);
    }
    else
    {
        AllFilterToggle->SetCheckedState(ECheckBoxState::Checked);
    }
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::OnConsumableFilterButtonToggled(bool bIsChecked)
{
    if (bIsChecked)
    {
        AllFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        MiscellaneousFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        EquipmentFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);

        OnItemFilterChanged.Broadcast(EItemFilter::Consumable);
    }
    else
    {
        ConsumableFilterToggle->SetCheckedState(ECheckBoxState::Checked);
    }
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::OnMiscellaneousFilterButtonToggled(bool bIsChecked)
{
    if (bIsChecked)
    {
        AllFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        ConsumableFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        EquipmentFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);

        OnItemFilterChanged.Broadcast(EItemFilter::Miscellaneous);
    }
    else
    {
        MiscellaneousFilterToggle->SetCheckedState(ECheckBoxState::Checked);
    }
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryWidget::OnEquipmentFilterButtonToggled(bool bIsChecked)
{
    if (bIsChecked)
    {
        AllFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        ConsumableFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);
        MiscellaneousFilterToggle->SetCheckedState(ECheckBoxState::Unchecked);

        OnItemFilterChanged.Broadcast(EItemFilter::Equipment);
    }
    else
    {
        EquipmentFilterToggle->SetCheckedState(ECheckBoxState::Checked);
    }
    return;
}
