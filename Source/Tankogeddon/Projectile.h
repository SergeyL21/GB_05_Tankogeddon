// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "Projectile.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed{ 100.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveRate{ 0.005f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float FlyRange{ 1000.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float PushForce{ 1000.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool bSingleImpact{ true };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ExplodeRadius{ 100.f };

	FTimerHandle MovementTimerHandle;
	FVector StartLocation;

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestoyedTarget, AActor*);
	FOnDestoyedTarget OnDestroyedTarget;

public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Start();
	virtual void Explode();
	void Stop();

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, 
							class AActor* OtherActor, 
							class UPrimitiveComponent* OtherComp, 
							int32 OtherBodyIndex, 
							bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	virtual void Move();
};
