// Fill out your copyright notice in the Description page of Project Settings.


#include "KillingObjective.h"
#include "DamageableObject.h"

// --------------------------------------------------------------------------------------
UKillingObjective::UKillingObjective()
{
	Type = EObjectiveType::Kill;
}

// --------------------------------------------------------------------------------------
void UKillingObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (auto DamageableTarget = Cast<IDamageableObject>(Target))
	{
		DamageableTarget->OnCriticalDamageTaken.AddLambda([this, Character](
			AActor* InstigatedActor, AActor* DamagedActor)
		{
			if (bCanBeCompleted && Character == DamagedActor)
			{
				bIsCompleted = true;

				if (++Killed >= TargetCount)
				{
					OnObjectiveCompleted.Broadcast(this);
				}
				else
				{
					const auto Percent {static_cast<float>(Killed) / TargetCount * 100.f};
					OnObjectiveInProgress.Broadcast(this, Percent);
				}
			}
		});
	}

	return;
}
