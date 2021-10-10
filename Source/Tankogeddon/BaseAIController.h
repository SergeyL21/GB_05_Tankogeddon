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
    ABasePawn* MyPawn;

    UPROPERTY()
    ABasePawn* EnemyPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float Accuracy = { 10.f };

public:
    ABasePawn* GetCurrentEnemy() const;
    void SetCurrentEnemy(ABasePawn* CurrentEnemy);
    void ResetCurrentEnemy();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void RotateToEnemy();
    //virtual bool IsEnemyPawnInRange() const;
    virtual bool DetectCanFire() const;
    virtual void Fire();
    virtual bool DetectEnemyVisibility() const;
};