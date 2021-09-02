// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "BaseAIController.h"
#include "TankAIController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankAIController : public ABaseAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    ATankPawn* TankPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
    TArray<FVector> PatrollingPoints;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI|Move params|Accurency")
    float MovementAccuracy{ 0.f };

    int32 CurrentPatrolPointIndex{ INDEX_NONE };

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    float CalculateRotationValue();
};
