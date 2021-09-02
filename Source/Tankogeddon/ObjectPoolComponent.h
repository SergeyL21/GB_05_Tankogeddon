// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "ObjectPoolComponent.generated.h"

class APooledActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TANKOGEDDON_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	TSubclassOf<APooledActor> PooledObjectClass;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int32 PoolSize{100};

	TArray<APooledActor*> Pool;

public:	
	// Sets default values for this component's properties
	UObjectPoolComponent();

	APooledActor* GetPooledObject();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
