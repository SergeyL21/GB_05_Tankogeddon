// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManagerComponent.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
    LocalInventoryComponent = InInventoryComponent;

    if (LocalInventoryComponent && InventoryItemsData)
    {
        // fill slot items info
        TArray<FInventoryItemInfo*> OutAllRows;
        FString Context;
        InventoryItemsData->GetAllRows<FInventoryItemInfo>(Context, OutAllRows);
        auto SlotIndex{ 0 };
        for (auto ItemInfo : OutAllRows)
        {
            FInventorySlotInfo SlotInfo;
            SlotInfo.ID = ItemInfo->ID;
            if (ItemInfo->Type == EItemType::Currency)
            {
                SlotInfo.Count = 0;
                LocalInventoryComponent->SetItem(-1, SlotInfo);
            }
            else
            {
                SlotInfo.Count = 1;
                LocalInventoryComponent->SetItem(SlotIndex++, SlotInfo);
            }
        }

        if (!InventoryWidget)
        {
            ensure(InventoryWidgetClass);
            InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
            InventoryWidget->OnItemDrop.AddUObject(this, &UInventoryManagerComponent::OnItemDropped);
            InventoryWidget->OnItemFilterChanged.AddUObject(this, &UInventoryManagerComponent::OnItemFilterChanged);
            //InventoryWidget->AddToViewport();
        }

        InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));
        FilterInventoryWidgetCells();
    }

    return;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::FilterInventoryWidgetCells(EItemFilter ItemFilter)
{
    if (LocalInventoryComponent && InventoryWidget)
    {
        InventoryWidget->ClearItems();

        for (auto& Item : LocalInventoryComponent->GetItems())
        {
            FInventoryItemInfo* ItemData = GetItemData(Item.Value.ID);
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
                    /*std::ignore = */ItemData->Icon.LoadSynchronous();
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
FInventoryItemInfo* UInventoryManagerComponent::GetItemData(FName ItemID)
{
    return InventoryItemsData ? InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, {}) : nullptr;
}

// --------------------------------------------------------------------------------------
void UInventoryManagerComponent::OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo)
{
    if (!DraggedFrom || !DroppedTo || !LocalInventoryComponent)
    {
        return;
    }

    LocalInventoryComponent->SetItem(DraggedFrom->IndexInInventory, DroppedTo->GetItem());
    LocalInventoryComponent->SetItem(DroppedTo->IndexInInventory, DraggedFrom->GetItem());

    auto FromSlot{ DraggedFrom->GetItem() };
    auto ToSlot{ DroppedTo->GetItem() };

    auto FromInfo{ GetItemData(FromSlot.ID) };
    auto ToInfo{ GetItemData(ToSlot.ID) };

    if (FromInfo == nullptr || (ToSlot.ID != NAME_None && ToInfo == nullptr))
    {
        return;
    }

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




