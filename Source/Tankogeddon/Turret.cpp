// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "Cannon.h"

// --------------------------------------------------------------------------------------
// Sets default values
ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);

	auto TurretMeshTemp{ LoadObject<UStaticMesh>(this, *TurretMeshPath) };
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	auto BodyMeshTemp{ LoadObject<UStaticMesh>(this, *BodyMeshPath) };
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}
}

// --------------------------------------------------------------------------------------
void ATurret::TakeDamage(const FDamageData& DamageData)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s has taken damage: %f"), *GetName(), DamageData.DamageValue);
	return;
}

// --------------------------------------------------------------------------------------
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, OUT Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(OUT TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

// --------------------------------------------------------------------------------------
void ATurret::Destroyed()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
}

// --------------------------------------------------------------------------------------
void ATurret::Targeting()
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if (CanFire() && Cannon && Cannon->IsReadyToFire())
	{
		Fire();
	}
}

// --------------------------------------------------------------------------------------
void ATurret::RotateToPlayer()
{
	auto targetRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation()) };
	auto currRotation{ TurretMesh->GetComponentRotation() };
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(currRotation, targetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

// --------------------------------------------------------------------------------------
bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

// --------------------------------------------------------------------------------------
bool ATurret::CanFire()
{
	auto TargetingDirection{ TurretMesh->GetForwardVector() };
	auto DirectionToPlayer{ PlayerPawn->GetActorLocation() - GetActorLocation() };
	DirectionToPlayer.Normalize();
	float AimAngle{ FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer))) };
	return AimAngle <= Accurency;
}

// --------------------------------------------------------------------------------------
void ATurret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}


