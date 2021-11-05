// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarkerActor.h"

// --------------------------------------------------------------------------------------
void ALocationMarkerActor::BeginPlay()
{
	Super::BeginPlay();

	SetMarkerVisibility(false);
	return;
}

// --------------------------------------------------------------------------------------
void ALocationMarkerActor::OnOverlapedByActor(AActor* OverlappingActor)
{
	if (OverlappingActor != nullptr)
	{
		NotifyOverlapWithActor(this, OverlappingActor);
	}
	return;
}

// --------------------------------------------------------------------------------------
void ALocationMarkerActor::SetMarkerVisibility(bool bEnabled)
{
	SetActorHiddenInGame(!bEnabled);
	return;
}
