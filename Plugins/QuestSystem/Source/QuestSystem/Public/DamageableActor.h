// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "DamageableObject.h"
#include "DamageableActor.generated.h"

UCLASS()
class QUESTSYSTEM_API ADamageableActor : public AActor, public IDamageableObject
{
	GENERATED_BODY()

public:
	virtual void TakeDamage_Implementation(AActor* InstigatedActor, AActor *DamagedActor) override;
	
	UFUNCTION(BlueprintCallable)
	void OnCritialDamageTaken(AActor* InstigatedActor, AActor *DamagedActor);
};
