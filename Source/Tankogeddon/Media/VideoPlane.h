// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "VideoPlane.generated.h"

UCLASS()
class TANKOGEDDON_API AVideoPlane : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMediaPlayer *MediaPlayer;
	
public:	
	// Sets default values for this actor's properties
	AVideoPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
