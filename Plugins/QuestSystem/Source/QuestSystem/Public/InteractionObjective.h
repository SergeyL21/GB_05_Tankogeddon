// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "Objective.h"
#include "InteractionObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()

public:
	UInteractionObjective();

	virtual void ActivateObjective(AActor * Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "InteractableObject"))
	AActor* Target;

	virtual TArray<AActor*> GetObjectiveActors() const override;
};
