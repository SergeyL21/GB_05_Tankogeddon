#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "GameStructs.h"
#include "Cannon.generated.h"

class UArrowComponent;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type {ECannonType::FireProjectile};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange{ 1000.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int MaxAmmo{ 10 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int FireShotNums{ 3 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireShotDelay{ 1.f };

	FTimerHandle ReloadTimerHandle;

	bool bReadyToFire{ false };
	int CurrentAmmo{ MaxAmmo };
	int CurrentShot{ 0 };

public:
	ACannon();

	bool Fire();
	bool FireSpecial();

	bool IsReadyToFire();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void Reload();

private:
	void SingleShot();
	void Reset();
};
