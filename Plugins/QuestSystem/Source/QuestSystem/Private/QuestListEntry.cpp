// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestListEntry.h"

#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>

#include <tuple>

// --------------------------------------------------------------------------------------
void UQuestListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectQuestButton)
	{
		SelectQuestButton->OnReleased.AddDynamic(this, &UQuestListEntry::SelectQuest);
	}

	return;
}

// --------------------------------------------------------------------------------------
void UQuestListEntry::SetQuestText(const FText& Text)
{
	if (QuestText)
	{
		QuestText->SetText(Text);
	}

	return;
}

// --------------------------------------------------------------------------------------
void UQuestListEntry::SetIsSelected(bool IsSelected)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetBrushColor(IsSelected ? 
			FLinearColor(1.f, 1.f, 1.f, 1.f) : FLinearColor(1.f, 1.f, 1.f, 0.f));
	}

	return;
}

// --------------------------------------------------------------------------------------
void UQuestListEntry::SelectQuest()
{
	std::ignore = OnQuestChosen.ExecuteIfBound();
	return;
}
