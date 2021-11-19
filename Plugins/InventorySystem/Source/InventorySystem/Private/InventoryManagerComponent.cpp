// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "EquipInventoryWidget.h"

#include <tuple>

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::InitLocalInventory(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (LocalInventoryComponent && InventoryItemsData)
    {
        if (!InventoryWidget)
        {
            ensure(InventoryWidgetClass);
            InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
            InventoryWidget->OnItemDrop.AddUObject(this, &ThisClass::OnItemDropped);
            InventoryWidget->OnItemFilterChanged.AddUObject(this, &ThisClass::OnItemFilterChanged);
            InventoryWidget->OnItemUsed.AddUObject(this, &ThisClass::OnItemUsed);
            InventoryWidget->RepresentedInventory = LocalInventoryComponent;
            //InventoryWidget->AddToViewport();
        }

        InventoryWidget->InitItems(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));
        FilterInventoryWidgetCells();
    }

    return;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::InitEquipment(UInventoryComponent* InInventoryComponent)
{
    ensure(EquipInventoryWidgetClass);
    EquipInventoryWidget = CreateWidget<UEquipInventoryWidget>(GetWorld(), EquipInventoryWidgetClass);
    EquipInventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
    EquipInventoryWidget->RepresentedInventory = InInventoryComponent;
    //EquipInventoryWidget->AddToViewport();
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::FilterInventoryWidgetCells(EItemFilter ItemFilter)
{
    if (LocalInventoryComponent && InventoryWidget)
    {
        InventoryWidget->ClearItems();

        for (auto& Item : LocalInventoryComponent->GetItems())
        {
            auto ItemData {GetItemData(Item.Value.ID)};
            if (ItemData)
            {
                bool bFilterFlag{ true };

                if (Item.Key != -1)
                {
                    switch (ItemFilter)
                    {
                    case EItemFilter::Consumable:
                        bFilterFlag = (ItemData->Type == EItemType::Consumable);
                        break;
                    case EItemFilter::Miscellaneous:
                        bFilterFlag = (ItemData->Type == EItemType::Miscellaneous);
                        break;
                    case EItemFilter::Equipment:
                        bFilterFlag = (ItemData->Type == EItemType::Equipment);
                        break;
                    case EItemFilter::None:
                        bFilterFlag = true;
                        break;
                    default:
                        bFilterFlag = false;
                        break;
                    }
                }

                if (bFilterFlag)
                {
                    std::ignore = ItemData->Icon.LoadSynchronous();
                    InventoryWidget->AddItem(Item.Value, *ItemData, Item.Key);
                }
            }
        }
    }

    return;
}

// --------------------------------------------------------------------------------------
bool UInventoryManagerComponent::InventoryWidgetIsVisibled() const
{
    return InventoryWidget ? InventoryWidget->IsInViewport() : false;
}

// --------------------------------------------------------------------------------------
bool UInventoryManagerComponent::EquipInventoryWidgetIsVisibled() const
{
    return EquipInventoryWidget ? EquipInventoryWidget->IsInViewport() : false;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::SetInventoryWidgetVisible(bool bVisible)
{
    if (InventoryWidget)
    {
        if (bVisible)
        {
            InventoryWidget->AddToViewport();
        }
        else
        {
            InventoryWidget->RemoveFromViewport();
        }
    }
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::SetEquipInventoryWidgetVisible(bool bVisible)
{
    if (EquipInventoryWidget)
    {
        if (bVisible)
        {
            EquipInventoryWidget->AddToViewport();
        }
        else
        {
            EquipInventoryWidget->RemoveFromViewport();
        }
    }
    return;
}

// --------------------------------------------------------------------------------------
FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
    return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, {}) : nullptr;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    if (!DraggedFrom || !DroppedTo)
    {
        return;
    }

    auto FromInventory{ DraggedFrom->ParentInventoryWidget->RepresentedInventory };
    auto ToInventory{ DroppedTo->ParentInventoryWidget->RepresentedInventory };

    if (!FromInventory || !ToInventory)
    {
        return;
    }

    auto FromSlot{ DraggedFrom->GetItem() };
    auto ToSlot{ DroppedTo->GetItem() };

    if (FromSlot.Count < 1)
    {
        return;
    }

    auto FromInfo{ GetItemData(FromSlot.ID) };
    auto ToInfo{ GetItemData(ToSlot.ID) };

    if (FromInfo == nullptr || (ToSlot.ID != NAME_None && ToInfo == nullptr))
    {
        return;
    }

    const auto MaxCount{ ToInventory->GetMaxItemAmount(DroppedTo->IndexInInventory, *FromInfo) };
    if (MaxCount == 0)
    {
        return;
    }
    else if (MaxCount > 0)
    {
        const auto ItemsToAdd{ FMath::Min(MaxCount, FromSlot.Count) };
        ToSlot.Count = FromSlot.Count - ItemsToAdd;
        ToSlot.ID = FromSlot.ID;
        ToInfo = FromInfo;

        FromSlot.Count = ItemsToAdd;
    }
    else if (FromSlot.ID == ToSlot.ID)
    {
        FromSlot.Count += ToSlot.Count;
        ToSlot.Count = 0;
    }

    FromInventory->SetItem(DraggedFrom->IndexInInventory, ToSlot);
    ToInventory->SetItem(DroppedTo->IndexInInventory, FromSlot);

    DraggedFrom->Clear();
    if (ToInfo)
    {
        DraggedFrom->AddItem(ToSlot, *ToInfo);
    }

    DroppedTo->Clear();
    DroppedTo->AddItem(FromSlot, *FromInfo);

    return;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::OnItemFilterChanged(EItemFilter ItemFilter)
{
    FilterInventoryWidgetCells(ItemFilter);
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::OnItemUsed(UInventoryCellWidget* CellWidget)
{
    auto SlotInfo {CellWidget->GetItem()};
    if (const auto ItemInfo = GetItemData(SlotInfo.ID))
    {
        if (ItemInfo->Type == EItemType::Consumable)
        {
            LocalInventoryComponent->SetItem(CellWidget->IndexInInventory, SlotInfo);
            OnConsumableItemUsed.Broadcast(ItemInfo);
        }
    }

    return;
}




