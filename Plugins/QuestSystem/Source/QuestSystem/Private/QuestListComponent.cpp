// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable All
#include "QuestListComponent.h"
#include "CurrentObjectives.h"
#include "Quest.h"

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UQuestListComponent::UQuestListComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
// Called when the game starts
void UQuestListComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentObjectivesWidgetClass)
	{
		auto ObjectivesWidget { CreateWidget<UCurrentObjectives>(GetWorld(), CurrentObjectivesWidgetClass)};
		OnActiveQuestChanged.AddUObject(ObjectivesWidget, &UCurrentObjectives::SetCurrentObjectives);
		ObjectivesWidget->AddToViewport();
	}
	
	return;
}

// --------------------------------------------------------------------------------------
// Called every frame
void UQuestListComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	return;
}

// --------------------------------------------------------------------------------------
void UQuestListComponent::AddQuest(AQuest* Quest)
{
	if (Quest != nullptr)
	{
		if (ActiveQuest == nullptr || ActiveQuest->IsCompleted())
		{
			ActiveQuest = Quest;
			OnActiveQuestChanged.Broadcast(ActiveQuest);
		}
		
		Quest->TakeQuest(GetOwner());
		Quest->OnQuestStatusUpdated.AddLambda([this](AQuest *ChangedQuest)
		{
			if (ActiveQuest == ChangedQuest)
			{
				OnActiveQuestChanged.Broadcast(ChangedQuest);
			}
		});
		AcceptedQuests.AddUnique(Quest);
	}
	return;
}

// --------------------------------------------------------------------------------------
const TArray<AQuest*>& UQuestListComponent::GetQuests() const
{
	return AcceptedQuests;
}

// --------------------------------------------------------------------------------------
AQuest* UQuestListComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

// --------------------------------------------------------------------------------------
void UQuestListComponent::SetActiveQuest(AQuest* Quest)
{
	if (!Quest || AcceptedQuests.Contains(Quest))
	{
		ActiveQuest = Quest;
		OnActiveQuestChanged.Broadcast(ActiveQuest);
	}
}

