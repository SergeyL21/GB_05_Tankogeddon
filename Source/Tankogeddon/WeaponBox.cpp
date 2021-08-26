// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBox.h"
#include "TankPawn.h"
#include "Cannon.h"

// --------------------------------------------------------------------------------------
// Sets default values
AWeaponBox::AWeaponBox()
{
	PrimaryActorTick.bCanEverTick = false;

	ensure(CannonClass);
}

// --------------------------------------------------------------------------------------
void AWeaponBox::MeshOverlapBeginImpl(ATankPawn* Pawn)
{
	if (ensure(Pawn)) {
		Pawn->SetupCurrentCannon(CannonClass);
	}
	return;
}



