// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable All
#include "CurrentObjectives.h"
#include "Quest.h"
#include "ObjectiveWidget.h"

#include <Components/VerticalBox.h>

// --------------------------------------------------------------------------------------
void UCurrentObjectives::SetCurrentObjectives(AQuest* Quest)
{
	if (ObjectivesList && ObjectiveWidgetClass)
	{
		ObjectivesList->ClearChildren();
		if (Quest)
		{
			for (auto Objective : Quest->GetObjectives())
			{
				auto ObjectiveWidget {CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass)};
				ObjectiveWidget->Init(Objective);
				/*auto Slot = */ObjectivesList->AddChildToVerticalBox(ObjectiveWidget);
			}
		}
	}

	return;
}
