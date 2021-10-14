// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "InventoryItemInfo.h"
#include "InventoryCellWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UCheckBox;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemFilterChanged, EItemFilter /*ItemFilter*/);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnItemDrop OnItemDrop;
    FOnItemFilterChanged OnItemFilterChanged;

protected:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ItemCellsGrid;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* AllFilterToggle;
    UPROPERTY(meta = (BindWidget))
    UCheckBox* EquipmentFilterToggle;
    UPROPERTY(meta = (BindWidget))
    UCheckBox* ConsumableFilterToggle;
    UPROPERTY(meta = (BindWidget))
    UCheckBox* MiscellaneousFilterToggle;

    UPROPERTY(EditAnywhere)
    int32 ItemsInRow{5};

    UPROPERTY(EditAnywhere)
    TSubclassOf<UInventoryCellWidget> CellWidgetClass;
    UPROPERTY()
    TArray<UInventoryCellWidget*> CellWidgets;
    UPROPERTY(meta = (BindWidgetOptional))
    UInventoryCellWidget* GoldCell;

public:
    virtual void NativeConstruct() override;

    void Init(int32 ItemsNum);

    bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition = -1);

    void ClearItem(int32 SlotPosition = -1);

    void ClearItems();

protected:
    UInventoryCellWidget* CreateCellWidget();

    void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

    UFUNCTION()
    void OnAllFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnEquipmentFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnConsumableFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnMiscellaneousFilterButtonToggled(bool bIsChecked);
};
