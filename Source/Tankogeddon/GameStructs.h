#pragma once

#include <CoreMinimal.h>
#include <Engine/UserDefinedStruct.h>
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8 
{
    FireProjectile = 0  UMETA(DisplayName = "Use projectile"),
    FireTrace = 1       UMETA(DisplayName = "Use trace")
};

USTRUCT()
struct FDamageData
{
    GENERATED_BODY()

    UPROPERTY()
    float DamageValue{ 0.f };

    UPROPERTY()
    AActor* Instigator {nullptr};

    UPROPERTY()
    AActor* DamageMaker {nullptr};

    UPROPERTY()
    FVector HitLocation;

    UPROPERTY()
    bool bOutIsFatalDamage{ false };
};