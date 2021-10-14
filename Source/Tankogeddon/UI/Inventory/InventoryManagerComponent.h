// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "InventoryItemInfo.h"
#include "InventoryManagerComponent.generated.h"

struct FInventoryItemInfo;

class UInventoryComponent;
class UInventoryCellWidget;
class UDataTable;
class UInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryManagerComponent : public UActorComponent
{
    GENERATED_BODY()

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

public:
    // Sets default values for this component's properties
    UInventoryManagerComponent();

    void Init(UInventoryComponent* InInventoryComponent);

    void FilterInventoryWidgetCells(EItemFilter ItemFilter = EItemFilter::None);

    bool InventoryWidgetIsVisibled() const;

    void SetInventoryWidgetVisible(bool bVisible);

    FORCEINLINE UInventoryComponent* GetLocalInventoryComponent() const { return LocalInventoryComponent; }

    FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

    FInventoryItemInfo* GetItemData(FName ItemID);

protected:
    void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

    void OnItemFilterChanged(EItemFilter ItemFilter);
};
