// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include "DamageTaker.h"

// InventorySystem plugin include section
#include "InventoryCellWidget.h"
#include "InventoryOwner.h"

#include "BasePawn.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UHealthComponent;
class UBoxComponent;
class USphereComponent;
class UParticleSystem;
class USoundBase;
class UWidgetComponent;
class UInventoryComponent;
class UInventoryManagerComponent;
class UEquipInventoryComponent;
class UInteractionComponent;
class UBarHPWidget;
class ACannon;
class ABaseBox;
class UPawnSaveGame;

UENUM()
enum class EPlayerGroupID {
	None,
	Group_01,
	Group_02
};

UCLASS()
class TANKOGEDDON_API ABasePawn : public APawn, public IDamageTaker, public IInventoryOwner
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
	UWidgetComponent* HealthWidgetComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* TargetingCollider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UInventoryManagerComponent* InventoryManagerComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UEquipInventoryComponent* EquipmentInventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> MainCannonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> SecondaryCannonClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestructionParticleSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* DestructionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Other")
	TSubclassOf<ABaseBox>  DestructionBonusBox;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSpeed{ 0.5f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	EPlayerGroupID PlayerGroupID {EPlayerGroupID::None};

private:
	UPROPERTY()
	ACannon* ActiveCannon {nullptr};

	UPROPERTY()
	ACannon* InactiveCannon { nullptr };

	UPROPERTY()
	UBarHPWidget* BarHPWidget { nullptr };

	UPROPERTY()
	FVector TurretTarget;

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
	virtual void ChangeWeapon();

	UFUNCTION()
	FVector GetTurretForwardVector() const;

	UFUNCTION()
	void SetTurretTarget(const FVector& TargetPosition);

	UFUNCTION()
	FVector GetEyesPosition() const;

	UFUNCTION(BlueprintCallable)
	bool IsPlayerPawn() const;

	FORCEINLINE UInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryManagerComponent;  }

	virtual void EquipItem(EEquipSlot EquipSlot, const FName& ID) override;

	virtual void UnequipItem(EEquipSlot EquipSlot, const FName& ID) override;

	TArray<UStaticMeshComponent*> GetEquipComponents(EEquipSlot EquipSlot) const;

	const UHealthComponent *GetHealthComponent() const { return HealthComponent; }

	UFUNCTION()
	virtual void SavePawnState(UPawnSaveGame* PawnSaveGame) const;

	UFUNCTION()
	virtual void LoadPawnState(const UPawnSaveGame* LevelLoadGame);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void TargetDestroyed(AActor* Target);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void Die();

	UFUNCTION()
	virtual void DamageTaken(float InDamage);

	UBarHPWidget* GetBarHPWidget() const { return BarHPWidget; }

	UFUNCTION()
	void OnTargetingOverlapBegin(UPrimitiveComponent* OverlappedComp,
								 AActor* OtherActor,
								 UPrimitiveComponent* OtherComp,
								 int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetingOverlapEnd(UPrimitiveComponent* OverlappedComp, 
							   AActor* OtherActor, 
							   UPrimitiveComponent* OtherComp, 
							   int32 OtherBodyIndex);
	
};
