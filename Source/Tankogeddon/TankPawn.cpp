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

// --------------------------------------------------------------------------------------
// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
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
	if (ensure(MainCannonClass))
	{
		SetupCurrentCannon(MainCannonClass);
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::SetupCurrentCannon(TSubclassOf<ACannon> InCannonClass)
{
	if (ActiveCannon)
	{
		ActiveCannon->Destroy();
		ActiveCannon = nullptr;
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	ActiveCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, params);
	ActiveCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::ChangeWeapon()
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
void ATankPawn::Fire()
{
	if (ActiveCannon)
	{
		ActiveCannon->Fire();
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::FireSpecial() {
	if (ActiveCannon)
	{
		ActiveCannon->FireSpecial();
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
	const auto currentLocation{ GetActorLocation() };
	const auto forwardVector{ GetActorForwardVector()};
	const auto movePosition{ currentLocation + forwardVector  * CurrentForwardAxisValue * MoveSpeed * DeltaTime};

	FHitResult* SweepHitResult{ nullptr };
	SetActorLocation(movePosition, true, SweepHitResult);
	if (SweepHitResult)
	{
		CurrentForwardAxisValue = 0.f;
	}
	DEBUG_MESSAGE(0, FColor::Yellow, "Location: %s", *movePosition.ToString())

	// Tank rotation
	CurrentRightAxisValue = FMath::FInterpTo(CurrentRightAxisValue, TargetRightAxisValue, DeltaTime, RotationSmootheness);
	auto yawRotation{ RotationSpeed * CurrentRightAxisValue * DeltaTime };
	const auto currentRotation{ GetActorRotation() };
	yawRotation += currentRotation.Yaw;
	const auto newRotation{ FRotator{0.f, yawRotation, 0.f} };
	SetActorRotation(newRotation);
	DEBUG_MESSAGE(1, FColor::Yellow, "Body Rotation: %f", newRotation.Yaw)

	// Turret rotation
	if (TankController)
	{
		const auto mousePos{ TankController->GetMousePos() };
		auto targetRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos) };
		const auto currentTurretRotation{ TurretMesh->GetComponentRotation() };
		targetRotation.Pitch = currentTurretRotation.Pitch;
		targetRotation.Roll = currentTurretRotation.Roll;
		const auto newTurretRotation{ FMath::RInterpConstantTo(currentTurretRotation, targetRotation, DeltaTime, TurretRotationSpeed) };
		TurretMesh->SetWorldRotation(newTurretRotation);
		DEBUG_MESSAGE(2, FColor::Yellow, "Turret Rotation: %f", newTurretRotation.Yaw)
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

