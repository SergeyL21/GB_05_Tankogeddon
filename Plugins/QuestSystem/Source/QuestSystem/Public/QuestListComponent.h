// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "QuestListComponent.generated.h"

class AQuest;
class UCurrentObjectives;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestListComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AQuest*> AcceptedQuests;

	UPROPERTY()
	AQuest* ActiveQuest;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCurrentObjectives> CurrentObjectivesWidgetClass;
	
public:
	// Sets default values for this component's properties
	UQuestListComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddQuest(AQuest* Quest);

	const TArray<AQuest*>& GetQuests() const;

	AQuest* GetActiveQuest() const;

	void SetActiveQuest(AQuest *Quest);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestAction, AQuest*);
	FOnQuestAction OnActiveQuestChanged;
};
