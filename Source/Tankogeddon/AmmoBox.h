// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseBox.h"
#include "AmmoBox.generated.h"

class ACannon;

UCLASS()
class TANKOGEDDON_API AAmmoBox : public ABaseBox
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 Count { 1 };

public:
	AAmmoBox();

protected:
	//virtual void BeginPlay() override;

	virtual void MeshOverlapBeginImpl(ATankPawn *Pawn) override;

};
