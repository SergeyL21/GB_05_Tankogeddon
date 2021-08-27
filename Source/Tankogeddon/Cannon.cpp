#include "Cannon.h"

#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include <TimerManager.h>
#include <Engine/Engine.h>

#include "Tankogeddon.h"
#include "Projectile.h"
#include "ActorPoolSubsystem.h"

// --------------------------------------------------------------------------------------
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

// --------------------------------------------------------------------------------------
bool ACannon::Fire()
{
	if (!bReadyToFire)
	{
		DEBUG_MESSAGE_EX(11, FColor::Green, "The cannon isn't ready to fire!");
		return false;
	}

	--CurrentAmmo;
	if (CurrentAmmo < 1)
	{
		DEBUG_MESSAGE_EX(11, FColor::Green, "There are no ammo!");
		bReadyToFire = true;
		return false;
	}

	bReadyToFire = false;
	DEBUG_MESSAGE_EX(11, FColor::Green, "Shooting process...");
	SingleShot();
	return true;
}

// --------------------------------------------------------------------------------------
bool ACannon::FireSpecial()
{
	// some special fire code
	return true;
}

// --------------------------------------------------------------------------------------
void ACannon::AddAmmo(int32 Count)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + Count, 0, MaxAmmo);
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

// --------------------------------------------------------------------------------------
bool ACannon::IsReadyToFire() const
{
	return bReadyToFire;
}

// --------------------------------------------------------------------------------------
void ACannon::Reload()
{
	if (CurrentShot >= NumShotsInSeries) 
	{
		Reset();
	}
	else 
	{
		SingleShot();
	}
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::SingleShot()
{
	++CurrentShot;
	auto Delay{ CurrentShot < NumShotsInSeries ? FireShotDelay : 1.f / FireRate };
	
	if (Type == ECannonType::FireProjectile)
	{
		float progress{ float(CurrentShot) / NumShotsInSeries * 100 };
		auto Pool{ GetWorld()->GetSubsystem<UActorPoolSubsystem>() };
		FTransform SpawnTransform{ ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector };
		auto Projectile{ Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform)) };
		if (Projectile)
		{
			Projectile->Start();
			DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - projectile [%d/%d] (progress %2.f%%)", CurrentAmmo, MaxAmmo, progress);
		}
	}
	else
	{
		FHitResult hitResult;
		auto traceParams{ FCollisionQueryParams(FName(TEXT("FireTrace")), true, this) };
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;

		auto start{ ProjectileSpawnPoint->GetComponentLocation() };
		auto end{ ProjectileSpawnPoint->GetForwardVector() * FireRange + start };
		if (GetWorld()->LineTraceSingleByChannel(OUT hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if (hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}
		DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - trace")
	}

	GetWorld()->GetTimerManager().SetTimer(OUT ReloadTimerHandle, this, &ACannon::Reload, Delay, false);
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::Reset()
{
	bReadyToFire = true;
	CurrentShot = 0;
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MaxAmmo;
	Reset();
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	return;
}


