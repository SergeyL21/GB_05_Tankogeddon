// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include "InventoryItemInfo.h"
#include "InventoryOwner.generated.h"

// ----------------- !!! ------------------
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IInventoryOwner
{
	GENERATED_BODY()

public:
	//virtual ~IInventoryOwner = default;

	virtual void EquipItem(EEquipSlot EquipSlot, const FName& ID) = 0;

	virtual void UnequipItem(EEquipSlot EquipSlot, const FName& ID) = 0;
};
