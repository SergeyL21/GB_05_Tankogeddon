// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <AIController.h>
#include "TankAIController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankAIController : public AAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    ATankPawn* TankPawn;

    UPROPERTY()
    APawn* PlayerPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
    TArray<FVector> PatrollingPoints;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params|Accurency")
    float MovementAccuracy{ 0.f };

    int32 CurrentPatrolPointIndex{ INDEX_NONE };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange{ 1000.f };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed{ 0.25f };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accuracy = { 10.f };

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    float CalculateRotationValue();
    void Targeting();
    void RotateToPlayer();
    bool IsPlayerInRange() const;
    bool DetectCanFire() const;
    void Fire();
    bool DetectPlayerVisibility() const;
};
