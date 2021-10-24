// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "InventoryComponent.h"
#include "InventoryItemInfo.h"
#include "EquipInventoryComponent.generated.h"

class IInventoryOwner;
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UEquipInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere)
    TMap<int32, EEquipSlot> EquipSlots;

    UPROPERTY()
    TScriptInterface<IInventoryOwner> InventoryOwner;

public:
    UEquipInventoryComponent();

    virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const override;

    virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item) override;

protected:
    virtual void BeginPlay() override;
};
