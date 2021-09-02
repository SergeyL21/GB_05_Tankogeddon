// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>

#include "Tankogeddon.h"
#include "Cannon.h"
#include "TankPlayerController.h"
#include "HealthComponent.h"

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

	HealthComponent->OnDie.AddDynamic(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ATankPawn::DamageTaken);
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

	TankController = Cast<ATankPlayerController>(GetController());
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::Die()
{
	Destroy();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::DamageTaken(float InDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taken damage: %f. HP left: %f"), *GetName(), InDamage, HealthComponent->GetHealth());
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::AddAmmoToWeapon(int32 Count)
{
	if (ActiveCannon) 
	{
		// TODO: check type weapon
		ActiveCannon->AddAmmo(Count);
	}

	return;
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
	DEBUG_MESSAGE(0, FColor::Yellow, "Location: %s", *MovePosition.ToString())

	// Tank rotation
	CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmootheness);
	auto YawRotation{ RotationSpeed * CurrentRightAxisValue * DeltaTime };
	const auto CurrentRotation{ GetActorRotation() };
	YawRotation += CurrentRotation.Yaw;
	const auto NewRotation{ FRotator{0.f, YawRotation, 0.f} };
	SetActorRotation(NewRotation);
	DEBUG_MESSAGE(1, FColor::Yellow, "Body Rotation: %f", NewRotation.Yaw)

	// Turret rotation
	if (TankController)
	{
		const auto MousePos{ TankController->GetMousePos() };
		auto TargetRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos) };
		const auto CurrentTurretRotation{ TurretMesh->GetComponentRotation() };
		TargetRotation.Pitch = CurrentTurretRotation.Pitch;
		TargetRotation.Roll = CurrentTurretRotation.Roll;
		const auto NewTurretRotation{ FMath::RInterpConstantTo(CurrentTurretRotation, TargetRotation, DeltaTime, TurretRotationSpeed) };
		TurretMesh->SetWorldRotation(NewTurretRotation);
		DEBUG_MESSAGE(2, FColor::Yellow, "Turret Rotation: %f", NewTurretRotation.Yaw)
	}
	return;
}

// --------------------------------------------------------------------------------------
// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	return;
}

