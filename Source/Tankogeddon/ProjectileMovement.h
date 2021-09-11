// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "ProjectileMovement.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UProjectileMovement : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	float Gravity{ -9.8f };

public:
	TArray<FVector> GenerateTrajectory(const FVector& StartPos, const FVector& StartVelocity, float MaxTime, float TimeStep, float MinZValue = 0.f);

};
