// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "InventoryItemInfo.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, FInventorySlotInfo> Items;

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual FInventorySlotInfo* GetItem(int32 SlotIndex);

	virtual void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item);

	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FInventoryItemInfo& Item) const;

	void ClearItem(int32 SlotIndex);

	const TMap<int32, FInventorySlotInfo> &GetItems() const;

	int32 GetItemsNum() const;
		
};
