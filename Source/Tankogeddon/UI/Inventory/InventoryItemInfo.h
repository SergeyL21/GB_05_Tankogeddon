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
enum class EEquipSlot : uint8
{
    None,
    Armor,
    Weapon_Left,
    Weapon_Right,
    Chassis,
    ControlBlock
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
    UPROPERTY(EditAnywhere, Category = "Typing")
    EEquipSlot EquipSlot;

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
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Ammo;
    UPROPERTY(EditAnywhere, Category = "Stats")
    float Health;
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