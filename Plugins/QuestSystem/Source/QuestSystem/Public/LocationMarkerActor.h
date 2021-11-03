// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "LocationMarker.h"
#include "LocationMarkerActor.generated.h"

UCLASS()
class QUESTSYSTEM_API ALocationMarkerActor : public AActor, public ILocationMarker
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void OnOverlapedByActor(AActor* OverlappingActor);
};
