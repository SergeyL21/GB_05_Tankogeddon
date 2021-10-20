// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "BaseInventoryWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UCheckBox;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UInventoryWidget : public UBaseInventoryWidget
{
    GENERATED_BODY()

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

    UPROPERTY(meta = (BindWidgetOptional))
    UInventoryCellWidget* GoldCell;

public:
    virtual void NativeConstruct() override;

    void InitItems(int32 ItemsNum);

    bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition = -1);

    void ClearItem(int32 SlotPosition = -1);

    void ClearItems();

protected:
    UFUNCTION()
    void OnAllFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnEquipmentFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnConsumableFilterButtonToggled(bool bIsChecked);
    UFUNCTION()
    void OnMiscellaneousFilterButtonToggled(bool bIsChecked);
};
