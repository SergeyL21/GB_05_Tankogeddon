// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include "DamageableObject.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamageableObject : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCriticalDamageTaken,
	AActor* /*InstigatedActor*/,
	AActor* /*DamagedActor*/
);

/**
 * 
 */
class QUESTSYSTEM_API IDamageableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be 
	// inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(AActor* InstigatedActor, AActor* DamagedActor);
	
	UFUNCTION()
	virtual void TakeDamage_Implementation(AActor* InstigatedActor, AActor* DamagedActor) = 0;

	virtual void NotifyCriticalDamageTaken(AActor* InstigatedActor, AActor* DamagedActor);
	
	FOnCriticalDamageTaken OnCriticalDamageTaken;
};
