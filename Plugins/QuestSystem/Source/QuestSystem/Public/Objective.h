// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>
#include "Objective.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
	Location,
	Interact,
	Collect,
	Kill
};

class UObjective;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjective* /*Objective*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectiveInProgress, UObjective* /*Objective*/, float /*Percent*/);

/**
 * 
 */
UCLASS(Abstract)
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE virtual void ActivateObjective(AActor * Character) {}

	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(VisibleAnywhere)
	EObjectiveType Type;

	FOnObjectiveCompleted OnObjectiveCompleted;
	FOnObjectiveInProgress OnObjectiveInProgress;

	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted {false};
	
	UPROPERTY(VisibleAnywhere)
	bool bCanBeCompleted {true};
};
