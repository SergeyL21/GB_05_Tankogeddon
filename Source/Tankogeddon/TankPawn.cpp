// Fill out your copyright notice in the Description page of Project Settings.


#include "Tankogeddon.h"

#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "TankPawn.h"
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
}

// --------------------------------------------------------------------------------------
void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

// --------------------------------------------------------------------------------------
void ATankPawn::RotateRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankPlayerController>(GetController());
}

// --------------------------------------------------------------------------------------
// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank movement
	const auto currentLocation{ GetActorLocation() };
	const auto forwardVector{ GetActorForwardVector() * TargetForwardAxisValue };
	const auto movePosition{ currentLocation + forwardVector  *  MoveSpeed * DeltaTime};
	SetActorLocation(movePosition, true);
	DEBUG_MESSAGE(0, "Location: %s", *movePosition.ToString())

	// Tank rotation
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);
	auto yawRotation{ RotationSpeed * CurrentRightAxisValue * DeltaTime };
	const auto currentRotation{ GetActorRotation() };
	yawRotation += currentRotation.Yaw;
	const auto newRotation{ FRotator{0.f, yawRotation, 0.f} };
	SetActorRotation(newRotation);
	DEBUG_MESSAGE(1, "Body Rotation: %f", newRotation.Yaw)

	// Turret rotation
	if (TankController)
	{
		const auto mousePos{ TankController->GetMousePos() };
		auto targetRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos) };
		const auto currentTurretRotation{ TurretMesh->GetComponentRotation() };
		targetRotation.Pitch = currentTurretRotation.Pitch;
		targetRotation.Roll = currentTurretRotation.Roll;
		const auto newTurretRotation{ FMath::Lerp(currentTurretRotation, targetRotation, TurretRotationInterpolationKey) };
		TurretMesh->SetWorldRotation(newTurretRotation);
		DEBUG_MESSAGE(2, "Turret Rotation: %f", newTurretRotation.Yaw)
	}
}

// --------------------------------------------------------------------------------------
// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

