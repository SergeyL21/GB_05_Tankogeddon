// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableObject.h"

// --------------------------------------------------------------------------------------
// Add default functionality here for any IDamageableObject functions that are not pure virtual.
void IDamageableObject::NotifyCriticalDamageTaken(AActor* InstigatedActor, AActor* DamagedActor)
{
	if (OnCriticalDamageTaken.IsBound())
	{
		OnCriticalDamageTaken.Broadcast(InstigatedActor, DamagedActor);
	}
}
