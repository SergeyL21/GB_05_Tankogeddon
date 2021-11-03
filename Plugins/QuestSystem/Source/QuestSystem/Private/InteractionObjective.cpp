// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionObjective.h"
#include "InteractableObject.h"

// --------------------------------------------------------------------------------------
UInteractionObjective::UInteractionObjective()
{
	Type = EObjectiveType::Interact;
}

// --------------------------------------------------------------------------------------
void UInteractionObjective::ActivateObjective(AActor* Character)
{
	if (auto InteractableTarget = Cast<IInteractableObject>(Target))
	{
		InteractableTarget->OnInteractionFinished.AddLambda([this, Character](
			AActor* InteractableObject, AActor* ActorInteractedWithObject)
		{
			if (bCanBeCompleted && Character == ActorInteractedWithObject)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}

	return;
}
