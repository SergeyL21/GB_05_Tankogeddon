// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include "DamageTaker.h"
#include "BasePawn.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UHealthComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UAudioComponent;
class ACannon;
class ABaseBox;

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn, public IDamageTaker
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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DeathParticleEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* DeathAudioEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> MainCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> SecondaryCannonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UForceFeedbackEffect* DamageForceEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> DamageShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Other")
	TSubclassOf<ABaseBox> DropBoxClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSpeed{ 0.5f };

	UPROPERTY()
	ACannon* ActiveCannon {nullptr};
	UPROPERTY()
	ACannon* InactiveCannon {nullptr};

	int32 ScorePoints{ 0 };
	bool bIsActiveState{ true };

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	virtual void TakeDamage(FDamageData& DamageData) override;

	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void FireSpecial();

	UFUNCTION()
	void SetupCurrentCannon(TSubclassOf<ACannon> InCannonClass);

	UFUNCTION()
	ACannon* GetActiveCannon() const;

	UFUNCTION()
	void ChangeWeapon();

	UFUNCTION()
	FVector GetTurretForwardVector() const;

	UFUNCTION()
	void RotateTurretTo(const FVector& TargetPosition);

	UFUNCTION()
	FVector GetEyesPosition() const;

	UFUNCTION()
	bool IsPlayerPawn() const;

	UFUNCTION()
	void DropLoot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void TargetDestroyed(AActor* Target);

private:
	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaken(float InDamage);
};
