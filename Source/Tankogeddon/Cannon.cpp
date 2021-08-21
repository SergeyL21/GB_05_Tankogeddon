#include "Cannon.h"

#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>
#include <TimerManager.h>
#include <Engine/Engine.h>

#include "Tankogeddon.h"

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
void ACannon::Fire()
{
	if (!bReadyToFire) {
		return;
	}
	bReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{
		DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - projectile")
	}
	else
	{
		DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - trace")
	}

	GetWorld()->GetTimerManager().SetTimer(OUT ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::FireSpecial()
{
	if (!bReadyToFire) {
		return;
	}
	bReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{
		DEBUG_MESSAGE_EX(10, FColor::Green, "Alt Fire - projectile")
	}
	else
	{
		DEBUG_MESSAGE_EX(10, FColor::Green, "Alt Fire - trace")
	}

	GetWorld()->GetTimerManager().SetTimer(OUT ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	return;
}

// --------------------------------------------------------------------------------------
bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

// --------------------------------------------------------------------------------------
void ACannon::Reload()
{
	bReadyToFire = true;
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	return;
}


