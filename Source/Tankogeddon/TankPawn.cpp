// Fill out your copyright notice in the Description page of Project Settings.


#include "Tankogeddon.h"
#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

void ATankPawn::MoveForward(float AxisValue)
{
	targetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	targetRightAxisValue = AxisValue;
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO: implement the correct way to bind tank to the floor.
	auto currentLocation{ GetActorLocation() };
	currentLocation.Z = 0;
	SetActorLocation(currentLocation);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto currentLocation{ GetActorLocation() };
	const auto forwardVector{ GetActorForwardVector() * targetForwardAxisValue };
	const auto movePosition{ currentLocation + forwardVector  * MoveSpeed * DeltaTime};
	SetActorLocation(movePosition, true);
	DEBUG_MESSAGE(0, "Location: %s", *movePosition.ToString())

	auto yawRotation{ RotationSpeed * targetRightAxisValue * DeltaTime };
	const auto currentRotation{ GetActorRotation() };
	yawRotation += currentRotation.Yaw;
	const auto newRotation{ FRotator{0.f, yawRotation, 0.f} };
	SetActorRotation(newRotation);
	DEBUG_MESSAGE(0, "Rotation: %f", newRotation.Yaw)
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

