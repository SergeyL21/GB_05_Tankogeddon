// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleManager.h>

class FToolBarBuilder;
class FMenuBuilder;
class AQuest;

class FHelperEditorWindowModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply OnQuestButtonClicked();

	TSharedRef<ITableRow> OnGenerateRowForQuestList(
		AQuest *Quest,
		const TSharedRef<STableViewBase>& OwnerTable
	);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TArray<AQuest*> Quests;
	TSharedPtr<SListView<AQuest*>> QuestListViewPtr;
};
