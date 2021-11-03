// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarkerActor.h"

// --------------------------------------------------------------------------------------
void ALocationMarkerActor::OnOverlapedByActor(AActor* OverlappingActor)
{
	NotifyOverlapWithActor(this, OverlappingActor);
	return;
}
