// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperEditorWindow.h"
#include "HelperEditorWindowStyle.h"
#include "HelperEditorWindowCommands.h"
#include "MultiColumnQuestRow.h"
#include <LevelEditor.h>
#include <Widgets/Docking/SDockTab.h>
#include <Widgets/Text/STextBlock.h>
#include <ToolMenus.h>
#include <Engine/Selection.h>

// --------------------------------------------------------------------------------------
static const FName HelperEditorWindowTabName("HelperEditorWindow");

#define LOCTEXT_NAMESPACE "FHelperEditorWindowModule"

// --------------------------------------------------------------------------------------
void FHelperEditorWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FHelperEditorWindowStyle::Initialize();
	FHelperEditorWindowStyle::ReloadTextures();

	FHelperEditorWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	
	PluginCommands->MapAction(
		FHelperEditorWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FHelperEditorWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHelperEditorWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(HelperEditorWindowTabName, FOnSpawnTab::CreateRaw(this, &FHelperEditorWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FHelperEditorWindowTabTitle", "HelperEditorWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FHelperEditorWindowStyle::Shutdown();

	FHelperEditorWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(HelperEditorWindowTabName);

	return;
}

// --------------------------------------------------------------------------------------
TSharedRef<SDockTab> FHelperEditorWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	/*FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FHelperEditorWindowModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("HelperEditorWindow.cpp"))
		);*/

	auto QuestButtonText {FText::FromString("Find Quests")};

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.Padding(10.f, 10.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SButton)
					.ForegroundColor(FSlateColor{FLinearColor{0.f, 0.33f, 0.66f, 1.f}})
					.OnClicked_Raw(this, &FHelperEditorWindowModule::OnQuestButtonClicked)
					[
						SNew(STextBlock)
						.Text(QuestButtonText)
					]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(QuestListViewPtr, SListView<AQuest*>)
					.ItemHeight(28.f)
					.ListItemsSource(&Quests)
					.OnGenerateRow_Raw(this, &FHelperEditorWindowModule::OnGenerateRowForQuestList)
					.HeaderRow
					 (
						 SNew(SHeaderRow)
						 + SHeaderRow::Column("Actor_Name")
						 [
							 SNew(SBorder)
							 .HAlign(HAlign_Center)
							 .Padding(1.f)
							 .Content()
							 [
								 SNew(STextBlock)
								 .ColorAndOpacity(FLinearColor{0.8f, 0.4f, 0.f, 1.f})
								 .Text(FText::FromString("Actor Name"))
							 ]
						 ]
						 
						 + SHeaderRow::Column("Name")
						 [
							 SNew(SBorder)
							 .HAlign(HAlign_Center)
							 .Padding(1.f)
							 .Content()
							 [
								 SNew(STextBlock)
								 .Text(FText::FromString("Quest Name"))
							 ]
						 ]

						 + SHeaderRow::Column("Description")
						 [
							 SNew(SBorder)
							 .HAlign(HAlign_Center)
							 .Padding(1.f)
							 .Content()
							 [
								 SNew(STextBlock)
								 .Text(FText::FromString("Quest Description"))
							 ]
						 ]
						 
						 + SHeaderRow::Column("IsStoryQuest")
						 [
							 SNew(SBorder)
							 .HAlign(HAlign_Center)
							 .Padding(1.f)
							 .Content()
							 [
								 SNew(STextBlock)
								 .Text(FText::FromString("Is Story"))
							 ]
						 ]
						
					 )
				]
			+ SVerticalBox::Slot()
				[
					SNew(SSpacer)
				]
				
		];
}

// --------------------------------------------------------------------------------------
FReply FHelperEditorWindowModule::OnQuestButtonClicked()
{
	if (GEditor == nullptr)
	{
		return FReply::Unhandled();
	}
	
	TArray<AActor*> UnmatchedActors;
	Quests.Empty();
	for (FSelectionIterator Iter((GEditor->GetSelectedActorIterator())); Iter; ++Iter)
	{
		if (auto Actor = Cast<AActor>(*Iter))
		{
			if (auto Quest = Cast<AQuest>(Actor))
			{
				Quests.AddUnique(Quest);
			}
			else
			{
				UnmatchedActors.AddUnique(Actor);
			}
		}
	}

	for (auto UnmatchedActor : UnmatchedActors)
	{
		GEditor->SelectActor(UnmatchedActor, false, false);
	}
	
	if (QuestListViewPtr)
	{
		QuestListViewPtr->RequestListRefresh();
	}
	
	return FReply::Handled();
}

// --------------------------------------------------------------------------------------
TSharedRef<ITableRow> FHelperEditorWindowModule::OnGenerateRowForQuestList(AQuest *Quest,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(SMultiColumnQuestRow, OwnerTable).Item(Quest);
}

// --------------------------------------------------------------------------------------
void FHelperEditorWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(HelperEditorWindowTabName);
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FHelperEditorWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FHelperEditorWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}

	return;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHelperEditorWindowModule, HelperEditorWindow)