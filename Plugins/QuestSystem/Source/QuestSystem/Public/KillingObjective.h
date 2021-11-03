// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "Objective.h"
#include "KillingObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UKillingObjective : public UObjective
{
	GENERATED_BODY()

public:
	UKillingObjective();

	virtual void ActivateObjective(AActor * Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "DamageableObject"))
	AActor* Target;

	UPROPERTY(EditAnywhere)
	int32 TargetCount {0};

private:
	int32 Killed {0};
};
