// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPoolComponent.h"
#include "PooledActor.h"
#include <UObject/ConstructorHelpers.h>
#include <Engine.h>

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UObjectPoolComponent::UObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
APooledActor* UObjectPoolComponent::GetPooledObject()
{
	return *Pool.FindByPredicate([](const auto Element) { return !Element->IsActive(); });
}

// --------------------------------------------------------------------------------------
// Called when the game starts
void UObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != PooledObjectClass) 
	{
		Pool.Reserve(PoolSize);
		for (int32 n{ 0 }; n < PoolSize; ++n)
		{
			auto Actor{ GetWorld()->SpawnActor<APooledActor>(PooledObjectClass, FVector{}.ZeroVector, FRotator{}.ZeroRotator) };
			Actor->SetActive(false);
			Pool.Add(Actor);
		}
	}
}

