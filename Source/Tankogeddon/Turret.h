#pragma once

#include <CoreMinimal.h>
#include "BasePawn.h"
#include "Scorable.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UHealthComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATurret : public ABasePawn, public IScorable
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    APawn* PlayerPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange{ 1000.f };
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed{ 0.1f };
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRate{ 0.005f };
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accurency{ 10.f };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Scorable")
    int32 DestructionScores{ 3 };

    const FString BodyMeshPath{ "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'" };
    const FString TurretMeshPath{ "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'" };

public:
    ATurret();

protected:
    virtual void BeginPlay() override;

    virtual int32 GetScorePoints() const override;

    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    
    bool CanFire() const;
};

