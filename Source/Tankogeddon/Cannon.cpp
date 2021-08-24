#include "Cannon.h"

#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>
#include <TimerManager.h>
#include <Engine/Engine.h>

#include "Tankogeddon.h"
#include "Projectile.h"

// --------------------------------------------------------------------------------------
ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

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
bool ACannon::IsReadyToFire()
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
		auto projectile{ GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation())};
		if (projectile) {
			projectile->Start();
		}
		DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - projectile [% i / % i] (progress %2.f%%)", CurrentAmmo, MaxAmmo, progress);
	}
	else
	{
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


