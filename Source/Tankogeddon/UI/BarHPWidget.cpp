// Fill out your copyright notice in the Description page of Project Settings.

#include "BarHPWidget.h"
#include <Components/ProgressBar.h>

// --------------------------------------------------------------------------------------
void UBarHPWidget::SetHP(float CurrentHealth, float MaxHealth)
{
	if (MainBar)
	{
		const auto Percent{ CurrentHealth / MaxHealth };
		MainBar->SetPercent(Percent);
	}
}
