// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"
#include "TankPawn.h"
#include "Cannon.h"

// --------------------------------------------------------------------------------------
// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
//void AAmmoBox::BeginPlay()
//{
//	ensure(CannonClass);
//}

// --------------------------------------------------------------------------------------
void AAmmoBox::MeshOverlapBeginImpl(ATankPawn *Pawn)
{
	if (ensure(Pawn)) {
		Pawn->AddAmmoToWeapon(Count);
	}
	return;
}



