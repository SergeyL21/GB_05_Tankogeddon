// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include "DamageTaker.h"
#include "BaseShootingPawn.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UHealthComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ABaseShootingPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	float TurretRotationSpeed{ 0.5f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> MainCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> SecondaryCannonClass;

	UPROPERTY()
	ACannon* ActiveCannon {nullptr};
	UPROPERTY()
	ACannon* InactiveCannon {nullptr};

public:
	// Sets default values for this pawn's properties
	ABaseShootingPawn();

	virtual void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION()
	virtual bool CanFire() const;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void FireSpecial();

	UFUNCTION()
	void SetupCurrentCannon(TSubclassOf<ACannon> InCannonClass, bool bForceInactive = false);

	UFUNCTION()
	virtual void Die();

	UFUNCTION()
	virtual void DamageTaken(float InDamage);

	UFUNCTION()
	void ChangeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
};
