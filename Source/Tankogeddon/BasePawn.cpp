// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"

#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <Components/WidgetComponent.h>

#include "Tankogeddon.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "BaseAIController.h"
#include "BaseBox.h"
#include "Scorable.h"

#include "UI/BarHPWidget.h"

// InventorySystem plugin include section
#include "InventoryComponent.h"
#include "InventoryManagerComponent.h"
#include "EquipInventoryComponent.h"
#include "InteractionComponent.h"
#include "InventoryWidget.h"

#include "Saving/LevelSaveGame.h"

// --------------------------------------------------------------------------------------
// Sets default values
ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->bEditableWhenInherited = true;
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);
	TurretMesh->bEditableWhenInherited = true;

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
	CannonSetupPoint->bEditableWhenInherited = true;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);
	HitCollider->bEditableWhenInherited = true;

	TargetingCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Vision collider"));
	TargetingCollider->SetupAttachment(BodyMesh);
	TargetingCollider->bEditableWhenInherited = true;
	TargetingCollider->OnComponentBeginOverlap.AddDynamic(this, &ABasePawn::OnTargetingOverlapBegin);
	TargetingCollider->OnComponentEndOverlap.AddDynamic(this, &ABasePawn::OnTargetingOverlapEnd);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ABasePawn::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ABasePawn::DamageTaken);
	HealthComponent->bEditableWhenInherited = true;

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarHP"));
	HealthWidgetComponent->SetupAttachment(BodyMesh);
	HealthWidgetComponent->SetWidgetClass(UBarHPWidget::StaticClass());
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManager");
	EquipmentInventoryComponent = CreateDefaultSubobject<UEquipInventoryComponent>("EquipInventory");

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("Interaction");
	InteractionComponent->SetupAttachment(BodyMesh);
}

// --------------------------------------------------------------------------------------
void ABasePawn::TakeDamage(FDamageData& DamageData)
{
	HealthComponent->TakeDamage(OUT DamageData);

	if (!IsPlayerPawn())
	{
		BarHPWidget->SetHP(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
	}
	return;
}

// --------------------------------------------------------------------------------------
// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto TargetRotation{ UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTarget) };
	auto CurrentRotation{ TurretMesh->GetComponentRotation() };
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TurretRotationSpeed));

	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::Fire()
{
	if (ActiveCannon && ActiveCannon->IsReadyToFire())
	{
		ActiveCannon->Fire();
	}
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::FireSpecial()
{
	if (ActiveCannon && ActiveCannon->IsReadyToFire())
	{
		ActiveCannon->FireSpecial();
	}
	return;
}

// --------------------------------------------------------------------------------------
ACannon* ABasePawn::GetActiveCannon() const
{
	return ActiveCannon;
}

// --------------------------------------------------------------------------------------
void ABasePawn::SetupCurrentCannon(TSubclassOf<ACannon> InCannonClass)
{
	if (!InCannonClass)
	{
		return;
	}

	if (ActiveCannon)
	{
		ActiveCannon->Destroy();
		ActiveCannon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	ActiveCannon = GetWorld()->SpawnActor<ACannon>(InCannonClass, Params);
	ActiveCannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ActiveCannon->OnDestroyedTarget.AddUObject(this, &ABasePawn::TargetDestroyed);

	if (IsPlayerPawn())
	{
		auto PlayerController{ Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController()) };
		PlayerController->SetCannonTextBlock(ActiveCannon->GetCannonName());
	}
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::Die()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionParticleSystem, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructionSound, GetActorLocation());

	if (DestructionBonusBox)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		GetWorld()->SpawnActor<ABaseBox>(DestructionBonusBox, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
	}

	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->ReleaseResources();
	}
	
	Destroy();
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::DamageTaken(float InDamage)
{
	UE_LOG(LogTemp, Log, TEXT("Pawn %s taken damage: [%f/%f] "), *GetName(), InDamage, HealthComponent->GetHealth());
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::OnTargetingOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (auto OtherPawn = Cast<ABasePawn>(OtherActor))
	{
		if (OtherPawn != this && OtherPawn->PlayerGroupID != PlayerGroupID)
		{
			if (auto AIController{ Cast<ABaseAIController>(GetController()) })
			{
				AIController->SetCurrentEnemy(OtherPawn);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::OnTargetingOverlapEnd(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (auto OtherPawn = Cast<ABasePawn>(OtherActor))
	{
		if (auto AIController{ Cast<ABaseAIController>(GetController()) })
		{
			AIController->ResetCurrentEnemy();
		}
	}
}

// --------------------------------------------------------------------------------------
void ABasePawn::ChangeWeapon()
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
FVector ABasePawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

// --------------------------------------------------------------------------------------
void ABasePawn::SetTurretTarget(const FVector& TargetPosition)
{
	TurretTarget = TargetPosition;
	return;
}

// --------------------------------------------------------------------------------------
FVector ABasePawn::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}

// --------------------------------------------------------------------------------------
bool ABasePawn::IsPlayerPawn() const
{
	return (Cast<APawn>(this) == GetWorld()->GetFirstPlayerController()->GetPawn());
}

// --------------------------------------------------------------------------------------
void ABasePawn::EquipItem(EEquipSlot EquipSlot, const FName& ID)
{
	if (auto Data = InventoryManagerComponent->GetItemData(ID))
	{
		for (auto StaticMeshComp : GetEquipComponents(EquipSlot))
		{
			StaticMeshComp->SetStaticMesh(Data->Mesh.LoadSynchronous());
		}

		//Damage += Data->Damage;
		//Armor += Data->Armor;
		//Intelligence += Data->Intelligence;
	}
	
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::UnequipItem(EEquipSlot EquipSlot, const FName& ID)
{
	for (auto StaticMeshComp : GetEquipComponents(EquipSlot))
	{
		StaticMeshComp->SetStaticMesh(nullptr);
	}
	
	//if (auto Data = InventoryManagerComponent->GetItemData(ID))
	//{
	//	Damage += Data->Damage;
	//	Armor += Data->Armor;
	//	Intelligence += Data->Intelligence;
	//}
	
	return;
}

// --------------------------------------------------------------------------------------
TArray<UStaticMeshComponent*> ABasePawn::GetEquipComponents(EEquipSlot EquipSlot) const
{
	TArray<UStaticMeshComponent*> Result;
	FName Tag;
	switch (EquipSlot)
	{
	case EEquipSlot::Weapon_Right:
		Tag = "EquipWeapon_Right";
		break;
	case EEquipSlot::Weapon_Left:
		Tag = "EquipWeapon_Left";
		break;
	case EEquipSlot::Armor:
		Tag = "EquipArmor";
		break;
	case EEquipSlot::Chassis:
		Tag = "EquipChassis";
		break;
	default:
		return Result;
	}

	auto Comps {GetComponentsByTag(UStaticMeshComponent::StaticClass(), Tag)};
	for (auto Comp : Comps)
	{
		if (auto SM_Comp = Cast<UStaticMeshComponent>(Comp))
		{
			Result.Add(Cast<UStaticMeshComponent>(Comp));
		}
	}
	return Result;
}

// --------------------------------------------------------------------------------------
void ABasePawn::SavePawnState(UPawnSaveGame* PawnSaveGame) const
{
	if (PawnSaveGame)
	{
		PawnSaveGame->BasePawnClass = this->GetClass();
		PawnSaveGame->CurrentHealth = HealthComponent->GetHealth();
		PawnSaveGame->MaxHealth = HealthComponent->GetMaxHealth();
		if (ActiveCannon)
		{
			PawnSaveGame->CurrentAmmo = ActiveCannon->GetCurrentAmmo();
			PawnSaveGame->MaxAmmo = ActiveCannon->GetMaxAmmo();
		}
		PawnSaveGame->Transform = GetActorTransform();

		if (InventoryComponent)
		{
			PawnSaveGame->InventoryItems = InventoryComponent->GetItems();
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::LoadPawnState(const UPawnSaveGame* PawnLoadGame)
{
	if (PawnLoadGame)
	{
		HealthComponent->SetHealth(PawnLoadGame->CurrentHealth);
		HealthComponent->SetMaxHealth(PawnLoadGame->MaxHealth);
		if (ActiveCannon)
		{
			ActiveCannon->SetAmmo(PawnLoadGame->CurrentAmmo);
			ActiveCannon->SetMaxAmmo(PawnLoadGame->MaxAmmo);
		}
		SetActorTransform(
			PawnLoadGame->Transform,
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);

		auto InventoryWidget {InventoryManagerComponent->GetInventoryWidget()};
		if (InventoryManagerComponent && InventoryComponent && InventoryWidget)
		{
			InventoryComponent->ClearItems();
			InventoryWidget->ClearItems();
			for (const auto& Pair : PawnLoadGame->InventoryItems)
			{
				const auto ItemData {InventoryManagerComponent->GetItemData(Pair.Value.ID)};
				InventoryComponent->SetItem(Pair.Key, Pair.Value);
				InventoryWidget->AddItem(Pair.Value, *ItemData, Pair.Key);
			}
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensure(MainCannonClass))
	{
		SetupCurrentCannon(SecondaryCannonClass);
		ChangeWeapon();
		SetupCurrentCannon(MainCannonClass);
	}

	if (!IsPlayerPawn())
	{
		BarHPWidget = Cast<UBarHPWidget>(HealthWidgetComponent->GetUserWidgetObject());
		if (BarHPWidget)
		{
			BarHPWidget->SetHP(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
		}
	}
	else if (InventoryManagerComponent)
	{
		InventoryManagerComponent->InitLocalInventory(InventoryComponent);
		if (IsPlayerPawn())
		{
			InventoryManagerComponent->InitEquipment(EquipmentInventoryComponent);
		}
	}

	return;
}

// -------------------------------------------------------------------------------------- 
void ABasePawn::Destroyed() 
{ 
	if (ActiveCannon) 
	{ 
		ActiveCannon->Destroy(); 
	} 
	if (InactiveCannon) 
	{ 
		InactiveCannon->Destroy(); 
	}

	if (HealthWidgetComponent->GetWidget())
	{
		HealthWidgetComponent->GetWidget()->Destruct();
	}
	
	return; 
} 

// --------------------------------------------------------------------------------------
void ABasePawn::TargetDestroyed(AActor* Target)
{
	return;
}


