// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTankWidget.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Animation/WidgetAnimation.h>

// --------------------------------------------------------------------------------------
void UPlayerTankWidget::NativeConstruct()
{
	Super::NativeConstruct();

	return;
}

// --------------------------------------------------------------------------------------
void UPlayerTankWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth, bool bAnimation)
{
	const auto Percent{ CurrentHealth / MaxHealth };
	PlayerHealthBar->SetPercent(Percent);

	if (PlayerHealthText)
	{
		PlayerHealthText->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentHealth, MaxHealth));
	}

	if (bAnimation && PlayerTakenDamageAnimation)
	{
		PlayAnimation(PlayerTakenDamageAnimation);
	}

	return;
}

// --------------------------------------------------------------------------------------
void UPlayerTankWidget::UpdateCannonInfo(const FString& CannonName)
{
	PlayerCannonText->SetText(FText::FromString(CannonName));
	return;
}

// --------------------------------------------------------------------------------------
void UPlayerTankWidget::UpdateAmmoInfo(float CurrentAmmo, float MaxAmmo)
{
	const auto Percent{ CurrentAmmo / MaxAmmo };
	PlayerAmmoBar->SetPercent(Percent);

	if (PlayerAmmoText)
	{
		PlayerAmmoText->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));
	}
	return;
}


