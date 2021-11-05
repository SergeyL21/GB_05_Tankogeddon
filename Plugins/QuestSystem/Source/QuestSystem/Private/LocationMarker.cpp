// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationMarker.h"

// --------------------------------------------------------------------------------------
void ILocationMarker::NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor)
{
	if (OnLocationReached.IsBound())
	{
		OnLocationReached.Broadcast(LocationMarker, OverlappedActor);
	}

	return;
}

