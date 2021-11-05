// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectionObjective.h"
#include "InteractableObject.h"

// --------------------------------------------------------------------------------------
UCollectionObjective::UCollectionObjective()
{
	Type = EObjectiveType::Collect;
}

// --------------------------------------------------------------------------------------
void UCollectionObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	for (auto Target : Targets)
	{
		if (auto InteractableTarget = Cast<IInteractableObject>(Target))
		{
			InteractableTarget->OnInteractionFinished.AddLambda([this, Character](
				AActor* InteractableObject, AActor* ActorInteractedWithObject)
			{
				if (bCanBeCompleted && Character == ActorInteractedWithObject)
				{
					bIsCompleted = true;

					if (++Collected >= TargetCount)
					{
						OnObjectiveCompleted.Broadcast(this);
					}
					else
					{
						const auto Percent {static_cast<float>(Collected) / TargetCount * 100.f};
						OnObjectiveInProgress.Broadcast(this, Percent);
					}
				}
			});
		}
	}

	return;
}
