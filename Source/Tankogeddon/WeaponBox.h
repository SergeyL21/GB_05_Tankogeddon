// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseBox.h"
#include "WeaponBox.generated.h"

class ACannon;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AWeaponBox : public ABaseBox
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;

public:
	AWeaponBox();

protected:
	//virtual void BeginPlay() override;

	virtual void MeshOverlapBeginImpl(ATankPawn* Pawn) override;
};
