// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Components/ArrowComponent.h>
#include <Engine/TargetPoint.h>

#include "Tankogeddon.h"
#include "Cannon.h"
#include "Scorable.h"
#include "TankogeddonGameModeBase.h"
#include "HealthComponent.h"

#define CURRENT_GAME_MODE (Cast<ATankogeddonGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))

// --------------------------------------------------------------------------------------
// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// --------------------------------------------------------------------------------------
void ATankPawn::MoveForward(float AxisValue)
{
    TargetForwardAxisValue = AxisValue;
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::RotateRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
	return;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	if (IsPlayerPawn())
	{
		CURRENT_GAME_MODE->PlayerUpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
		if (auto Cannon = GetActiveCannon())
		{
			CURRENT_GAME_MODE->PlayerChangeCannon(Cannon->GetCannonName());
			CURRENT_GAME_MODE->PlayerUpdateAmmoBar(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::Fire()
{
	Super::Fire();

	if (auto Cannon = GetActiveCannon())
	{
		CURRENT_GAME_MODE->PlayerUpdateAmmoBar(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::ChangeWeapon()
{
	Super::ChangeWeapon();
	
	if (auto Cannon = GetActiveCannon())
	{
		CURRENT_GAME_MODE->PlayerChangeCannon(Cannon->GetCannonName());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::DamageTaken(float DamageValue)
{
	Super::DamageTaken(DamageValue);

	if (IsPlayerPawn())
	{
		if (HitForceEffect)
		{
			FForceFeedbackParameters ShootForceEffectParams;
			ShootForceEffectParams.bLooping = false;
			ShootForceEffectParams.Tag = "ShootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(HitForceEffect, ShootForceEffectParams);
		}

		if (HitShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake);
		}

		CURRENT_GAME_MODE->PlayerUpdateHealthBar(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::TargetDestroyed(AActor* Target)
{
	if (IScorable* Scorable = Cast<IScorable>(Target))
	{
		AccumulatedScores += Scorable->GetScorePoints();
		UE_LOG(LogTemp, Log, TEXT("Destroyed target %s. Current scores: %d"), *Target->GetName(), AccumulatedScores);
	}
}

// --------------------------------------------------------------------------------------
void ATankPawn::Die()
{
	// generate event
	if (IsPlayerPawn())
	{
		CURRENT_GAME_MODE->OnPlayerDie();
	}

	Super::Die();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::AddAmmoToWeapon(int32 Count)
{
	auto Cannon{ GetActiveCannon() };
	if (Cannon) 
	{
		// TODO: check type weapon
		Cannon->AddAmmo(Count);

		CURRENT_GAME_MODE->PlayerUpdateAmmoBar(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
	}

	return;
}

// --------------------------------------------------------------------------------------
TArray<FVector> ATankPawn::GetPatrollingPoints() const
{
	TArray<FVector> Result;
	for (ATargetPoint* Point : PatrollingPoints)
	{
		Result.Add(Point->GetActorLocation());
	}

	return Result;
}

// --------------------------------------------------------------------------------------
void ATankPawn::SetPatrollingPoints(const TArray<ATargetPoint*>& NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}

// --------------------------------------------------------------------------------------
// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank movement
	CurrentForwardAxisValue = FMath::FInterpTo(CurrentForwardAxisValue, TargetForwardAxisValue, DeltaTime, MovementSmootheness);
	const auto CurrentLocation{ GetActorLocation() };
	const auto ForwardVector{ GetActorForwardVector()};
	const auto MovePosition{ CurrentLocation + ForwardVector  * CurrentForwardAxisValue * MoveSpeed * DeltaTime};

	FHitResult* SweepHitResult{ nullptr };
	SetActorLocation(MovePosition, true, SweepHitResult);
	if (SweepHitResult)
	{
		CurrentForwardAxisValue = 0.f;
	}

	// Tank rotation
	CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmootheness);
	auto YawRotation{ RotationSpeed * CurrentRightAxisValue * DeltaTime };
	const auto CurrentRotation{ GetActorRotation() };
	YawRotation += CurrentRotation.Yaw;
	const auto NewRotation{ FRotator{0.f, YawRotation, 0.f} };
	SetActorRotation(NewRotation);

	return;
}

// --------------------------------------------------------------------------------------
// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	return;
}
