// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"

#include <Components/StaticMeshComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/WidgetComponent.h>
#include <Engine/TargetPoint.h>

#include "Cannon.h"
#include "Scorable.h"
#include "TankPlayerController.h"
#include "HealthComponent.h"

#include "UI/BarHPWidget.h"

// InventorySystem plugin include section
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "InventoryManagerComponent.h"
#include "QuestList.h"
#include "QuestListComponent.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

#include "Saving/LevelSaveGame.h"

#define PLAYER_CONTROLLER Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController())

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

	QuestList = CreateDefaultSubobject<UQuestListComponent>("Quest List Component");

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	HealthWidgetComponent->SetupAttachment(SpringArm);

	InventoryManagerComponent->OnConsumableItemUsed.AddUObject(this, &ATankPawn::OnConsumableItemUse);
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
		PLAYER_CONTROLLER->SetHealthWidgetValue(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
		if (auto Cannon = GetActiveCannon())
		{
			PLAYER_CONTROLLER->SetCannonTextBlock(Cannon->GetCannonName());
			PLAYER_CONTROLLER->SetAmmoWidgetValue(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
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
		PLAYER_CONTROLLER->SetAmmoWidgetValue(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::ChangeWeapon()
{
	Super::ChangeWeapon();
	
	if (auto Cannon = GetActiveCannon())
	{
		PLAYER_CONTROLLER->SetCannonTextBlock(Cannon->GetCannonName());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::ToggleQuestListVisibility()
{
	auto PlayerController {UGameplayStatics::GetPlayerController(GetWorld(), 0)};

	if (QuestListWidget)
	{
		QuestListWidget->RemoveFromParent();
		QuestListWidget = nullptr;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		if (QuestListClass)
		{
			QuestListWidget = CreateWidget<UQuestList>(GetWorld(), QuestListClass);
			QuestListWidget->Init(QuestList);
			QuestListWidget->AddToViewport();
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController);
			PlayerController->bShowMouseCursor = true;
		}
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

		PLAYER_CONTROLLER->SetHealthWidgetValue(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
	else if (auto HPWidget = GetBarHPWidget())
	{
		HPWidget->SetHP(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
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

		if (InventoryManagerComponent && IsPlayerPawn())
		{
			auto LocalComponent{ InventoryManagerComponent->GetLocalInventoryComponent() };
			if (auto LocalSlotInfo = LocalComponent->GetItem(-1))
			{
				if (auto LocalItemInfo = InventoryManagerComponent->GetItemData(LocalSlotInfo->ID))
				{
					LocalSlotInfo->Count = AccumulatedScores;
					auto InventoryWidget{ InventoryManagerComponent->GetInventoryWidget() };
					InventoryWidget->ClearItem(-1);
					InventoryWidget->AddItem(*LocalSlotInfo, *LocalItemInfo);
				}
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::Die()
{
	// generate event
	if (IsPlayerPawn())
	{
		PLAYER_CONTROLLER->Die();
	}

	Super::Die();
	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::OnConsumableItemUse(const FInventoryItemInfo* ItemInfo)
{
	if (ItemInfo && IsPlayerPawn())
	{
		if (ItemInfo->Health > 0.f)
		{
			AddHealth(ItemInfo->Health);
		}

		if (ItemInfo->Ammo > 0.f)
		{
			AddAmmoToWeapon(ItemInfo->Ammo);
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::AddAmmoToWeapon(int32 Count)
{
	if (auto Cannon = GetActiveCannon())
	{
		// TODO: check type weapon
		Cannon->AddAmmo(Count);

		PLAYER_CONTROLLER->SetAmmoWidgetValue(Cannon->GetCurrentAmmo(), Cannon->GetMaxAmmo());
	}

	return;
}

// --------------------------------------------------------------------------------------
void ATankPawn::AddHealth(float Count)
{
	if (HealthComponent)
	{
		HealthComponent->AddHealth(Count);
		PLAYER_CONTROLLER->SetHealthWidgetValue(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
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
