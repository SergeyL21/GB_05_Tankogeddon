// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    if (!bCustomizeItems && InventorySlotsData)
    {
        // fill slot items info
        TArray<FInventorySlotInfo*> OutAllRows;
        InventorySlotsData->GetAllRows<FInventorySlotInfo>({}, OutAllRows);
        auto SlotIndex{ 0 };
        for (auto SlotInfo : OutAllRows)
        {
            SetItem(SlotIndex++, *SlotInfo);
        }
    }
    
	return;
}

// --------------------------------------------------------------------------------------
FInventorySlotInfo* UInventoryComponent::GetItem(int32 SlotIndex)
{
    return Items.Find(SlotIndex);
}

// --------------------------------------------------------------------------------------
void UInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
    ClearItem(SlotIndex);
    Items.Add(SlotIndex, Item);
    return;
}

// --------------------------------------------------------------------------------------
int32 UInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
    const auto InfoPtr{ Items.Find(SlotIndex) };
    return (InfoPtr && InfoPtr->ID != Item.ID) ? 0 : -1;
}

// --------------------------------------------------------------------------------------
void UInventoryComponent::ClearItem(int32 SlotIndex)
{
    Items.Remove(SlotIndex);
    return;
}

// --------------------------------------------------------------------------------------
void UInventoryComponent::ClearItems()
{
    Items.Empty();
    return;
}

// --------------------------------------------------------------------------------------
const TMap<int32, FInventorySlotInfo>& UInventoryComponent::GetItems() const
{
    return Items;
}

// --------------------------------------------------------------------------------------
int32 UInventoryComponent::GetItemsNum() const
{
    return Items.Num();
}

