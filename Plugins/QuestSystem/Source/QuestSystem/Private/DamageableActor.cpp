// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableActor.h"

// --------------------------------------------------------------------------------------
void ADamageableActor::TakeDamage_Implementation(AActor* InstigatedActor, AActor* DamagedActor)
{
	return;
}

// --------------------------------------------------------------------------------------
void ADamageableActor::OnCritialDamageTaken(AActor* InstigatedActor, AActor* DamagedActor)
{
	NotifyCriticalDamageTaken(InstigatedActor, DamagedActor);

	return;
}
