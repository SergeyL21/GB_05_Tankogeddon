#include "Cannon.h"

#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include <TimerManager.h>
#include <Engine/Engine.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/AudioComponent.h>
#include <Camera/CameraShake.h>
#include <GameFramework/ForceFeedbackEffect.h>

#include "Tankogeddon.h"
#include "Projectile.h"
#include "DamageTaker.h"
#include "Scorable.h"
#include "BasePawn.h"
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

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio effect"));
	AudioEffect->SetupAttachment(ProjectileSpawnPoint);
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
		bReadyToFire = true;
		return false;
	}

	bReadyToFire = false;
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

//UE_LOG(LogTemp, Warning, TEXT("Actor %s is destroyed by cannon %s"), *Actor->GetName(), *GetName());

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
			ShootEffect->ActivateSystem();
			AudioEffect->Play();

			Projectile->SetInstigator(GetInstigator());
			Projectile->OnDestroyedTarget.AddUObject(this, &ACannon::NotifyTargetDestroyed);
			Projectile->Start();
			DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - projectile [%d/%d] (progress %2.f%%)", CurrentAmmo, MaxAmmo, progress);
		}
	}
	else
	{
		FHitResult HitResult;
		auto TraceParams{ FCollisionQueryParams(FName(TEXT("FireTrace")), true, this) };
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;

		auto Start{ ProjectileSpawnPoint->GetComponentLocation() };
		auto End{ ProjectileSpawnPoint->GetForwardVector() * FireRange + Start };
		if (GetWorld()->LineTraceSingleByChannel(OUT HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			bool bWasTargetDestroyed{ false };
			if (HitResult.Component.IsValid())
			{
				auto HitActor{ HitResult.Actor.Get() };
				if (HitResult.Component->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
				{
					HitActor->Destroy();
					bWasTargetDestroyed = true;
				}
				else if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(HitActor))
				{
					auto CurrentInstigator{ GetInstigator() };
					if (HitActor != CurrentInstigator)
					{
						FDamageData DamageData;
						DamageData.DamageValue = FireDamage;
						DamageData.DamageMaker = this;
						DamageData.Instigator = CurrentInstigator;
						DamageTaker->TakeDamage(OUT DamageData);
						bWasTargetDestroyed = DamageData.bOutIsFatalDamage;
					}
				}

				if (bWasTargetDestroyed)
				{
					NotifyTargetDestroyed(HitResult.Actor.Get());
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 5);
		}
		DEBUG_MESSAGE_EX(10, FColor::Green, "Fire - trace [%d/%d]", CurrentAmmo, MaxAmmo);
	}

	GetWorld()->GetTimerManager().SetTimer(OUT ReloadTimerHandle, this, &ACannon::Reload, Delay, false);
	return;
}

// --------------------------------------------------------------------------------------
void ACannon::NotifyTargetDestroyed(AActor* Target)
{
	if (OnDestroyedTarget.IsBound())
	{
		OnDestroyedTarget.Broadcast(Target);
	}
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


