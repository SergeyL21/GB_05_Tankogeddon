// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "PooledActor.generated.h"

UCLASS()
class TANKOGEDDON_API APooledActor : public AActor
{
	GENERATED_BODY()
	
protected:
	FTimerHandle LifespanTimerHandle;
	float Lifespan{ 1.f };
	bool Active;

public:	
	// Sets default values for this actor's properties
	APooledActor();

	virtual void SetLifeSpan(float InLifespan) override;
	void SetActive(bool InActive);
	bool IsActive() const;

protected:
	void Deactivate();

};
