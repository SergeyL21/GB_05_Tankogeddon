// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <AIController.h>
#include "BaseAIController.generated.h"

class ABasePawn;
class APawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ABaseAIController : public AAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    ABasePawn* BasePawn;

    UPROPERTY()
    APawn* PlayerPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingRange{ 1000.f };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float TargetingSpeed{ 0.25f };

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accuracy = { 10.f };

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void Targeting();
    virtual void RotateToPlayer();
    virtual bool IsPlayerInRange() const;
    virtual bool DetectCanFire() const;
    virtual void Fire();
    virtual bool DetectPlayerVisibility() const;
};
