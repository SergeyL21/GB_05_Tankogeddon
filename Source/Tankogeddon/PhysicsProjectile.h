// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

class UProjectileMovement;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UProjectileMovement* PhysicsComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveAccurency{ 10.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationMaxTime{ 50.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationTimeStep{ 1.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	float TrajectorySimulationSpeed{ 20.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Trajectory")
	bool bShowTrajectory{ false };

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	FVector MoveVector;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	TArray<FVector> CurrentTrajectory;

	UPROPERTY(BlueprintReadWrite, Category = "Movement params")
	int32 TrajectoryPointIndex {0};

public:
	APhysicsProjectile();
	virtual void Start() override;

protected:
	virtual void Move() override;
};
