// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "InventoryItemInfo.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryComponent;
class UEquipInventoryComponent;
class UInventoryCellWidget;
class UDataTable;
class UInventoryWidget;
class UEquipInventoryWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConsumableItemUsed, const FInventoryItemInfo* /*ItemInfo*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnConsumableItemUsed OnConsumableItemUsed;

protected:
    UPROPERTY()
    UInventoryComponent* LocalInventoryComponent;

    UPROPERTY(EditAnywhere)
    UDataTable* InventoryItemsData;

    UPROPERTY()
    UInventoryWidget* InventoryWidget;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInventoryWidget> InventoryWidgetClass;
    UPROPERTY(EditAnywhere)
    int32 MinInventorySize {20};

    UPROPERTY()
    UEquipInventoryWidget* EquipInventoryWidget;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UEquipInventoryWidget> EquipInventoryWidgetClass;

public:
    // Sets default values for this component's properties
    UInventoryManagerComponent();

    void InitLocalInventory(UInventoryComponent* InInventoryComponent);

    void InitEquipment(UInventoryComponent* InInventoryComponent);

    void FilterInventoryWidgetCells(EItemFilter ItemFilter = EItemFilter::None);

    bool InventoryWidgetIsVisibled() const;

    bool EquipInventoryWidgetIsVisibled() const;

    void SetInventoryWidgetVisible(bool bVisible);

    void SetEquipInventoryWidgetVisible(bool bVisible);

    FORCEINLINE UInventoryComponent* GetLocalInventoryComponent() const { return LocalInventoryComponent; }

    FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

    FInventoryItemInfo* GetItemData(FName ItemID);

protected:
    void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);
    
    void OnItemFilterChanged(EItemFilter ItemFilter);

    void OnItemUsed(UInventoryCellWidget *CellWidget);
};
