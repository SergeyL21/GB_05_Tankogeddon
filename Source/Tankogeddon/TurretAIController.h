// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "BaseAIController.h"
#include "TurretAIController.generated.h"

class ATurret;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATurretAIController : public ABaseAIController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    ATurret* Turret;

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;
};
