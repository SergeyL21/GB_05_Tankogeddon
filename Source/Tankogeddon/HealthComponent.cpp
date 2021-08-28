// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

// --------------------------------------------------------------------------------------
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// --------------------------------------------------------------------------------------
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	return;
}

// --------------------------------------------------------------------------------------
void UHealthComponent::TakeDamage(const FDamageData& DamageData)
{
	float TakenDamageValue{ DamageData.DamageValue };
	CurrentHealth -= TakenDamageValue;

	if (CurrentHealth <= 0)
	{
		if (OnDie.IsBound())
		{
			OnDie.Broadcast();
		}
	}
	else
	{
		if (OnDamaged.IsBound())
		{
			OnDamaged.Broadcast(TakenDamageValue);
		}
	}

	return;
}

// --------------------------------------------------------------------------------------
float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

// --------------------------------------------------------------------------------------
float UHealthComponent::GetHealthState() const
{
	return CurrentHealth / MaxHealth;
}

// --------------------------------------------------------------------------------------
void UHealthComponent::AddHealth(float AddiditionalHealthValue)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + AddiditionalHealthValue, 0.f, MaxHealth);
}


