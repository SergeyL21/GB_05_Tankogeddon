// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "Quest.generated.h"

class AQuest;
class UObjective;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStatusUpdated, AQuest* /*Quest*/);

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

public:
	FOnQuestStatusUpdated OnQuestStatusUpdated;
	
protected:
	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(EditAnywhere)
	TArray<UObjective*> Objectives;
	
	UPROPERTY(EditAnywhere)
	bool bIsStoryQuest {true};
	
	UPROPERTY(EditAnywhere)
	bool bKeepObjectivesOrder {true};
	
	UPROPERTY(EditAnywhere)
	AQuest* PrerequisiteQuest;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsTaken {false};

	
public:	
	// Sets default values for this actor's properties
	AQuest();

	void TakeQuest(AActor * Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddLocationObjective();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddInteractObjective();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddCollectionObjective();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddKillingObjective();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void UpdateLocation();
	
	void OnObjectiveCompleted(UObjective* Objective);
};
