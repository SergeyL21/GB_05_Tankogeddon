// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestList.h"
#include "Quest.h"
#include "QuestListComponent.h"
#include "QuestListEntry.h"
#include "QuestDescription.h"

#include <Components/ScrollBox.h>
// --------------------------------------------------------------------------------------
void UQuestList::Init(UQuestListComponent* QuestList)
{
	if (QuestList && QuestEntryClass)
	{
		for (auto Quest : QuestList->GetQuests())
		{
			auto Scroll { Quest->IsStoryQuest() ? StoryQuestsList : SideQuestsList };
			if (Scroll)
			{
				auto QuestEntry { CreateWidget<UQuestListEntry>(this, QuestEntryClass) };
				QuestEntry->SetQuestText(Quest->GetQuestName());
				QuestEntry->OnQuestChosen.BindUObject(QuestList, &UQuestListComponent::SetActiveQuest, Quest);
				QuestEntries.Add(Quest, QuestEntry);
				Scroll->AddChild(QuestEntry);
			}
		}
		OnActiveQuestChanged(QuestList->GetActiveQuest());
		QuestList->OnActiveQuestChanged.AddUObject(this, &UQuestList::OnActiveQuestChanged);
	}

	return;
}

// --------------------------------------------------------------------------------------
void UQuestList::OnActiveQuestChanged(AQuest* ActiveQuest)
{
	for (const auto Pair : QuestEntries)
	{
		Pair.Value->SetIsSelected(Pair.Key == ActiveQuest);
	}
	if (ActiveQuestDescription)
	{
		ActiveQuestDescription->Init(ActiveQuest);
		ActiveQuestDescription->SetVisibility(ActiveQuest ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	return;
}
