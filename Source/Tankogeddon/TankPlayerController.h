// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/PlayerController.h>
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	FSimpleMulticastDelegate OnLeftMouseButtonUpEvent;

protected:
	UPROPERTY()
	ATankPawn* TankPawn;
	UPROPERTY()
	FVector MousePos;

public:
	ATankPlayerController();

	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	inline auto GetMousePos() { return MousePos; };

	void Die();
	void SetHealthWidgetValue(int32 CurrentHealth, int32 MaxHealth);
	void SetCannonTextBlock(const FString& CannonName);
	void SetAmmoWidgetValue(int32 CurrentAmmo, int32 MaxAmmo);
	void SetMinimapPosition(const FVector& WorldLocation, const FVector2D &WorldSize);

	void OnLeftMouseButtonUp();
	void OnLeftMouseButtonDown();

	void PauseMenuEnabled(bool bEnabled);

protected:
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void RotateRight(float AxisValue);
	void Fire();
	void FireSpecial();
	void ChangeWeapon();
	void InventorySwitch();
	void PauseMenuSwitch();

	UFUNCTION(exec)
	void DumpActorPoolSubsystemStats();
};
