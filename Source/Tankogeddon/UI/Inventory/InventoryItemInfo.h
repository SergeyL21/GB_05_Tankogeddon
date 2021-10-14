// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Engine/DataTable.h>
#include "InventoryItemInfo.generated.h"

UENUM()
enum class EItemType : uint8
{
    Undefined,
    Miscellaneous,
    Currency,
    Equipment,
    Consumable
};

UENUM()
enum class EItemRarity : uint8
{
    Common,
    Uncommon,
    Rare,
    Epic,
    Legendary
};

UENUM()
enum class EItemFilter : uint8 {
    None,
    Miscellaneous,
    Equipment,
    Consumable
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    // tech info
    UPROPERTY(EditAnywhere, Category = "General")
    FName ID;

    // text info
    UPROPERTY(EditAnywhere, Category = "General")
    FText Name;
    UPROPERTY(EditAnywhere, Category = "General")
    FText Description;

    // enums
    UPROPERTY(EditAnywhere, Category = "Typing")
    EItemType Type;
    UPROPERTY(EditAnywhere, Category = "Typing")
    EItemRarity Rarity;

    // visual representation
    UPROPERTY(EditAnywhere, Category = "Visual")
    TSoftObjectPtr<class UTexture2D> Icon;
    UPROPERTY(EditAnywhere, Category = "Visual")
    TSoftObjectPtr<class UStaticMesh> Mesh;

    // stats
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Damage;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Armor;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Intelligence;
};

USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
    FName ID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General")
    int32 Count;
};