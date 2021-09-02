// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "BaseBox.generated.h"

class UStaticMeshComponent;
class ATankPawn;

UCLASS()
class TANKOGEDDON_API ABaseBox : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

public:
	ABaseBox();

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult);

	virtual void MeshOverlapBeginImpl(ATankPawn *Pawn);
};
