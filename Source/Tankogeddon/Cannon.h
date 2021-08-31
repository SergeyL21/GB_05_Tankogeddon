#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "GameStructs.h"
#include "Cannon.generated.h"

class UArrowComponent;
class UObjectPoolComponent;
class AProjectile;

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
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type {ECannonType::FireProjectile};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange{ 1000.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	int32 MaxAmmo{ 10 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Fire params")
	int32 NumShotsInSeries{ 3 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "NumShotsInSeries > 1", EditConditionHides), Category = "Fire params")
	float FireShotDelay{ 1.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	bool bHasSpecialFire{ true };

	FTimerHandle ReloadTimerHandle;

	bool bReadyToFire{ false };
	int32 CurrentAmmo{ 0 };
	int32 CurrentShot{ 0 };

public:
	ACannon();

	bool Fire();
	bool FireSpecial();
	void AddAmmo(int32 Count);
	void SetVisibility(bool bIsVisible);

	bool IsReadyToFire() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestoyedTarget, AActor*);
	FOnDestoyedTarget OnDestroyedTarget;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void Reload();

	void NotifyTargetDestroyed(AActor* Target);

private:
	void SingleShot();
	void Reset();
};
