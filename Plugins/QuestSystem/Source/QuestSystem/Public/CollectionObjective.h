// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objective.h"
#include "CollectionObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UCollectionObjective : public UObjective
{
	GENERATED_BODY()

public:
	UCollectionObjective();

	virtual void ActivateObjective(AActor * Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "InteractableObject"))
	AActor* Target;

	UPROPERTY(EditAnywhere)
	int32 TargetCount {0};

	UPROPERTY(EditAnywhere)
	bool bAutoCollect {false};

private:
	int32 Collected {0};
};
