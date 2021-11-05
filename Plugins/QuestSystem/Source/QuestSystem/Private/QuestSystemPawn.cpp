// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemPawn.h"
#include "Quest.h"
#include "QuestListComponent.h"
#include "QuestDialog.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

// --------------------------------------------------------------------------------------
// Sets default values
AQuestSystemPawn::AQuestSystemPawn()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void AQuestSystemPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto Actor : AttachedActors)
	{
		if (auto Quest = Cast<AQuest>(Actor))
		{
			if (auto PrerequisiteQuest = Quest->GetPrerequisiteQuest())
			{
				Quest->SetVisibility(false);
				PrerequisiteQuest->OnQuestStatusUpdated.AddLambda([Quest](AQuest* PreviousQuest)
				{
					if (PreviousQuest->IsCompleted())
					{
						Quest->SetVisibility(true);
					}
				});
			}
		}
	}
	
	return;
}

// --------------------------------------------------------------------------------------
// Called every frame
void AQuestSystemPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	return;
}

// --------------------------------------------------------------------------------------
// Called to bind functionality to input
void AQuestSystemPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	return;
}

// --------------------------------------------------------------------------------------
void AQuestSystemPawn::Interact_Implementation(AActor* ActorInteractedWithObject)
{
	if (ActorInteractedWithObject != nullptr)
	{
		auto QuestListCmp {Cast<UQuestListComponent>(
			ActorInteractedWithObject->GetComponentByClass(UQuestListComponent::StaticClass()))};
		if (QuestListCmp == nullptr)
		{
			return;
		}

		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);
		for (auto Actor : AttachedActors)
		{
			if (auto Quest = Cast<AQuest>(Actor))
			{
				auto PrerequisiteQuest {Quest->GetPrerequisiteQuest()};
				if (Quest->IsTaken() || (PrerequisiteQuest && !PrerequisiteQuest->IsCompleted()))
				{
					continue;
				}

				if (QuestDialogClass)
				{
					auto QuestDialog {CreateWidget<UQuestDialog>(GetWorld(), QuestDialogClass)};
					QuestDialog->Init(Quest);
					QuestDialog->OnQuestAccepted.BindUObject(QuestListCmp, &UQuestListComponent::AddQuest, Quest);
					QuestDialog->OnDialogClosed.BindLambda([this, ActorInteractedWithObject]()
					{
						NotifyInteractionFinished(this, ActorInteractedWithObject);
					});
					QuestDialog->AddToViewport();
				}
			}
		}
	}

	NotifyInteractionFinished(this, ActorInteractedWithObject);
	return;
}


