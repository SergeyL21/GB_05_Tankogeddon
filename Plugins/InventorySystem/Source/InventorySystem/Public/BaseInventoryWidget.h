// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include "InventoryItemInfo.h"
#include "InventoryCellWidget.h"
#include "BaseInventoryWidget.generated.h"

class UInventoryCellWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemFilterChanged, EItemFilter /*ItemFilter*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUsed, UInventoryCellWidget* /*CellWidget*/)

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UBaseInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnItemDrop OnItemDrop;
	FOnItemFilterChanged OnItemFilterChanged;
	FOnItemUsed OnItemUsed;

public:
	UPROPERTY()
	class UInventoryComponent* RepresentedInventory;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TArray<UInventoryCellWidget*> CellWidgets;

public:
    virtual void NativeConstruct() override;

protected:
    UInventoryCellWidget* CreateCellWidget();

	void InitCellWidget(UInventoryCellWidget* Widget);

    void OnItemDropped(UInventoryCellWidget* DraggedFrom, UInventoryCellWidget* DroppedTo);

	void OnItemUsedFunc(UInventoryCellWidget *CellWidget);
};
