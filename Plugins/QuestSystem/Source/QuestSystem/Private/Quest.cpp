// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "Objective.h"
#include "LocationObjective.h"
#include "InteractionObjective.h"
#include "CollectionObjective.h"
#include "KillingObjective.h"

// --------------------------------------------------------------------------------------
// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
void AQuest::TakeQuest(AActor* Character)
{
	for (int32 i {0}; i < Objectives.Num(); ++i)
	{
		if (Objectives[i])
		{
			Objectives[i]->ActivateObjective(Character);
			Objectives[i]->bCanBeCompleted = !bKeepObjectivesOrder || i == 0;
			Objectives[i]->OnObjectiveCompleted.AddUObject(this, &AQuest::OnObjectiveCompleted);
		}
	}
	bIsTaken = true;
	return;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();

	return;
}

// --------------------------------------------------------------------------------------
void AQuest::AddLocationObjective()
{
	Objectives.Add(NewObject<ULocationObjective>(this));
	return;
}

// --------------------------------------------------------------------------------------
void AQuest::AddInteractObjective()
{
	Objectives.Add(NewObject<UInteractionObjective>(this));
	return;
}

// --------------------------------------------------------------------------------------
void AQuest::AddCollectionObjective()
{
	Objectives.Add(NewObject<UCollectionObjective>(this));
	return;
}

// --------------------------------------------------------------------------------------
void AQuest::AddKillingObjective()
{
	Objectives.Add(NewObject<UKillingObjective>(this));
	return;
}

// --------------------------------------------------------------------------------------
void AQuest::UpdateLocation()
{
	const auto ParentActor {GetAttachParentActor()};
	if (ParentActor)
	{
		SetActorLocation(ParentActor->GetActorLocation());
	}
	return;
}

// --------------------------------------------------------------------------------------
void AQuest::OnObjectiveCompleted(UObjective* Objective)
{
	if (bKeepObjectivesOrder)
	{
		int32 ObjectiveIndex;
		if (Objectives.Find(Objective, ObjectiveIndex) && 
			Objectives.IsValidIndex(ObjectiveIndex + 1))
		{
			Objectives[ObjectiveIndex + 1]->bCanBeCompleted = true;
		}
	}

	if (OnQuestStatusUpdated.IsBound())
	{
		OnQuestStatusUpdated.Broadcast(this);
	}
	return;
}



