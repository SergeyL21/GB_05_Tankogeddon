// Fill out your copyright notice in the Description page of Project Settings.

#include "Quest.h"
#include "Objective.h"
#include "LocationObjective.h"
#include "InteractionObjective.h"
#include "CollectionObjective.h"
#include "KillingObjective.h"

#include <Components/WidgetComponent.h>

// --------------------------------------------------------------------------------------
// Sets default values
AQuest::AQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = RootSceneComponent;
	
	InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestInfoWidget"));
	InfoWidgetComponent->SetupAttachment(RootComponent);
}

// --------------------------------------------------------------------------------------
void AQuest::TakeQuest(AActor* Character)
{
	for (int32 i {0}; i < Objectives.Num(); ++i)
	{
		if (Objectives[i])
		{
			Objectives[i]->bCanBeCompleted = !bKeepObjectivesOrder || i == 0;
			Objectives[i]->OnObjectiveCompleted.AddUObject(this, &AQuest::OnObjectiveCompleted);
			Objectives[i]->ActivateObjective(Character);
		}
	}
	bIsTaken = true;
	SetVisibility(false);
	
	return;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();

	/*if (auto ParentActor = GetAttachParentActor())
	{
		TakeQuest(ParentActor);
	}*/
	
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
			Objectives[ObjectiveIndex + 1]->PrepareObjective();
		}
	}
	
	OnQuestStatusUpdated.Broadcast(this);
	return;
}

// --------------------------------------------------------------------------------------
bool AQuest::IsCompleted() const
{
	const auto LastIndex {Objectives.Num() - 1 };
	if (bKeepObjectivesOrder && Objectives.IsValidIndex(LastIndex))
	{
		return Objectives[LastIndex]->bIsCompleted;
	}

	for (auto Objective : Objectives)
	{
		if (Objective && !Objective->bIsCompleted)
		{
			return false;
		}
	}

	return true;
}

// --------------------------------------------------------------------------------------
void AQuest::SetVisibility(bool bEnabled)
{
	InfoWidgetComponent->SetVisibility(bEnabled);
}



