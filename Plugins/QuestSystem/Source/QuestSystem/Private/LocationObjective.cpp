// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationObjective.h"
#include "LocationMarker.h"

// --------------------------------------------------------------------------------------
ULocationObjective::ULocationObjective()
{
	Type = EObjectiveType::Location;
}

// --------------------------------------------------------------------------------------
void ULocationObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	PrepareObjective();
	if (auto LocationMarker = Cast<ILocationMarker>(Marker))
	{
		LocationMarker->OnLocationReached.AddLambda([this, Character, LocationMarker](
			AActor* /*LocationMarker*/, AActor* OverlappedActor)
		{
			if (bCanBeCompleted && Character == OverlappedActor)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					LocationMarker->SetMarkerVisibility(false);
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}

	return;
}

// --------------------------------------------------------------------------------------
void ULocationObjective::PrepareObjective()
{
	if (auto LocationMarker = Cast<ILocationMarker>(Marker))
	{
		if (bCanBeCompleted)
		{
			LocationMarker->SetMarkerVisibility(true);
		}
	}
	return;
}

// --------------------------------------------------------------------------------------
TArray<AActor*> ULocationObjective::GetObjectiveActors() const
{
	return TArray<AActor*> { Marker };
}


