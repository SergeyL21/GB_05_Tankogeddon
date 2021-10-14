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
void UInventoryComponent::ClearItem(int32 SlotIndex)
{
    Items.Remove(SlotIndex);
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

