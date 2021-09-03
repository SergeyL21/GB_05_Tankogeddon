// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "BasePawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class UArrowComponent;
class UHealthComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed{ 100.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed{ 100.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MovementSmootheness{ 0.1f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSmootheness{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccuracy{ 10.f };

	int32 AccumulatedScores{ 0 };

	float TargetForwardAxisValue{ 0.f };
	float TargetRightAxisValue{ 0.f };
	float CurrentForwardAxisValue{ 0.f };
	float CurrentRightAxisValue{ 0.f };

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void RotateRight(float AxisValue);

	UFUNCTION()
	void AddAmmoToWeapon(int32 Count = 0);

	UFUNCTION()
	const TArray<FVector>& GetPatrollingPoints() const { return PatrollingPoints; }

	UFUNCTION() 
	float GetMovementAccurency() const { return MovementAccuracy; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TargetDestroyed(AActor* Target) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
