// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseShootingPawn.h"

#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>

#include "Tankogeddon.h"
#include "Cannon.h"
#include "HealthComponent.h"

// --------------------------------------------------------------------------------------
// Sets default values
ABaseShootingPawn::ABaseShootingPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::TakeDamage(const FDamageData& DamageData)
{
	HealthComponent->TakeDamage(DamageData);
	return;
}

// --------------------------------------------------------------------------------------
bool ABaseShootingPawn::CanFire() const
{
	return true;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::Fire()
{
	if (ActiveCannon)
	{
		ActiveCannon->Fire();
	}
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::FireSpecial()
{
	if (ActiveCannon)
	{
		ActiveCannon->FireSpecial();
	}
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::SetupCurrentCannon(TSubclassOf<ACannon> InCannonClass, bool bForceInactive)
{
	if (!InCannonClass)
	{
		return;
	}

	auto& LocalCannon{ bForceInactive ? InactiveCannon : ActiveCannon };
	if (LocalCannon)
	{
		LocalCannon->Destroy();
		LocalCannon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	LocalCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
	LocalCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::Die()
{
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::DamageTaken(float InDamage)
{
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::ChangeWeapon()
{
	Swap(ActiveCannon, InactiveCannon);

	if (ActiveCannon)
	{
		ActiveCannon->SetVisibility(true);
	}

	if (InactiveCannon)
	{
		InactiveCannon->SetVisibility(false);
	}

	return;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ABaseShootingPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensure(MainCannonClass))
	{
		SetupCurrentCannon(MainCannonClass, false);
		SetupCurrentCannon(SecondaryCannonClass, true);
	}
	return;
}

// --------------------------------------------------------------------------------------
void ABaseShootingPawn::Destroyed()
{
	if (ActiveCannon)
	{
		ActiveCannon->Destroy();
	}
	if (InactiveCannon)
	{
		InactiveCannon->Destroy();
	}
	return;
}


