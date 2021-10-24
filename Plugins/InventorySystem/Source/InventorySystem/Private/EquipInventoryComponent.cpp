// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipInventoryComponent.h"
#include "InventoryOwner.h"

// --------------------------------------------------------------------------------------
UEquipInventoryComponent::UEquipInventoryComponent()
{
    EquipSlots.Add(1, EEquipSlot::Armor);
    EquipSlots.Add(2, EEquipSlot::Weapon_Left);
    EquipSlots.Add(3, EEquipSlot::Weapon_Right);
    EquipSlots.Add(4, EEquipSlot::Chassis);
    EquipSlots.Add(5, EEquipSlot::ControlBlock);

    return;
}

// --------------------------------------------------------------------------------------
int32 UEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const
{
    const auto InfoPtr {Items.Find(SlotIndex)};
    if ((InfoPtr && InfoPtr->ID != NAME_None) ||
        Item.Type != EItemType::Equipment || 
        !EquipSlots.Contains(SlotIndex) || 
        EquipSlots.FindChecked(SlotIndex) != Item.EquipSlot)
    {
        return 0;
    }

    return 1;
}

// --------------------------------------------------------------------------------------
void UEquipInventoryComponent::SetItem(int32 SlotIndex, const FInventorySlotInfo& Item)
{
    const auto Equip{ GetItem(SlotIndex) };
    const auto EquipSlot{ EquipSlots.Contains(SlotIndex) ? EquipSlots.FindChecked(SlotIndex) : EEquipSlot::None };
    
    if (Equip && InventoryOwner.GetInterface())
    {
        InventoryOwner->UnequipItem(EquipSlot, Equip->ID);
    }
    else 
    {
        InventoryOwner->EquipItem(EquipSlot, Item.ID);
    }

    Super::SetItem(SlotIndex, Item);
    return;
}

// --------------------------------------------------------------------------------------
void UEquipInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    auto LocalInterface { Cast<IInventoryOwner>(GetOwner()) };
    ensure(LocalInterface);
    InventoryOwner.SetInterface(LocalInterface);
    InventoryOwner.SetObject(GetOwner());
    return;
}
