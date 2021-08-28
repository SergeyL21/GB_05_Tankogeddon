#pragma once

#include <CoreMinimal.h>
#include "BaseShootingPawn.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UHealthComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATurret : public ABaseShootingPawn
{
    GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* HitCollider;

    UPROPERTY()
    APawn* PlayerPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange{ 1000.f };
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRate{ 0.005f };
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accurency{ 10.f };

    const FString BodyMeshPath{ "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'" };
    const FString TurretMeshPath{ "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'" };

public:
    ATurret();

protected:
    virtual void BeginPlay() override;

    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange();
    
    virtual bool CanFire() const;

    void Die() override;

    void DamageTaken(float InDamage) override;
};

