// Fill out your copyright notice in the Description page of Project Settings.

#include "PooledActor.h"
#include <GameFramework/Actor.h>

// --------------------------------------------------------------------------------------
// Sets default values
APooledActor::APooledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
void APooledActor::SetLifeSpan(float InLifespan)
{
	Lifespan = InLifespan;
	GetWorld()->GetTimerManager().SetTimer(LifespanTimerHandle, this, &APooledActor::Deactivate, Lifespan, false);
}

// --------------------------------------------------------------------------------------
void APooledActor::SetActive(bool InActive)
{
	Active = InActive;
	SetActorHiddenInGame(!Active);
	UE_LOG(LogTemp, Warning, TEXT("Pooling actor active=%i name=%s"), Active, *GetActorLabel());
	return;
}

// --------------------------------------------------------------------------------------
bool APooledActor::IsActive() const
{
	return Active;
}

// --------------------------------------------------------------------------------------
void APooledActor::Deactivate()
{
	SetActive(false);
	
}

