// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"

#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>

#include "Tankogeddon.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "BaseAIController.h"
#include "BaseBox.h"

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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddDynamic(this, &ABasePawn::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &ABasePawn::DamageTaken);
	HealthComponent->bEditableWhenInherited = true;

	DeathParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	DeathParticleEffect->SetupAttachment(RootComponent);
	DeathParticleEffect->bEditableWhenInherited = true;
	DeathParticleEffect->bAutoActivate = false;

	DeathAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	DeathAudioEffect->SetupAttachment(RootComponent);
	DeathParticleEffect->bEditableWhenInherited = true;
}

// --------------------------------------------------------------------------------------
void ABasePawn::TakeDamage(FDamageData& DamageData)
{
	HealthComponent->TakeDamage(OUT DamageData);
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
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::Die()
{
	if (!IsPlayerPawn()) 
	{		
		//RootComponent->SetHiddenInGame(true);
		//TurretMesh->SetHiddenInGame(true);
		//CannonSetupPoint->SetHiddenInGame(true);

		if (ActiveCannon)
		{
			//ActiveCannon->SetActorHiddenInGame(true);
		}

		if (auto AIController = Cast<ABaseAIController>(GetController())) {
			SetActorTickEnabled(false);
			AIController->SetActorTickEnabled(false);
			DeathParticleEffect->ActivateSystem();
			DeathAudioEffect->Play();

			FTimerDelegate TimerCallback;
			TimerCallback.BindLambda([this]() 
				{ 
					DropLoot(); 
					Destroy(); 
				}
			);
			FTimerHandle DelayTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(OUT DelayTimerHandle, TimerCallback, 10.f, false);
			return;
		}
	}

	Destroy();
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::DamageTaken(float InDamage)
{
	if (IsPlayerPawn())
	{
		if (DamageForceEffect)
		{
			FForceFeedbackParameters ShootForceEffectParams;
			ShootForceEffectParams.bLooping = false;
			ShootForceEffectParams.Tag = "ShootForceEffectParams";
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(DamageForceEffect, ShootForceEffectParams);
		}

		if (DamageShake)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DamageShake);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Pawn %s taken damage: [%f/%f] "), *GetName(), InDamage, HealthComponent->GetHealth());
	return;
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
void ABasePawn::DropLoot()
{
	if (DropBoxClass)
	{
		FActorSpawnParameters Params;
		const auto Location{ GetActorLocation() };
		const auto Rotation{ GetActorRotation() };
		if (auto Box = GetWorld()->SpawnActor<ABaseBox>(DropBoxClass, Location, Rotation, OUT Params))
		{
			UE_LOG(LogTemp, Warning, TEXT("Box %s dropped"), *Box->GetName());
		}
	}
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
	return;
}

// --------------------------------------------------------------------------------------
void ABasePawn::TargetDestroyed(AActor* Target)
{
	return;
}


