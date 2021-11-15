// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperEditorModeEdModeToolkit.h"
#include "HelperEditorModeEdMode.h"
#include <Engine/Selection.h>
#include <Widgets/Text/STextBlock.h>
#include <EditorModeManager.h>
#include <Widgets/Colors/SColorPicker.h>

#define LOCTEXT_NAMESPACE "FHelperEditorModeEdModeToolkit"

// --------------------------------------------------------------------------------------
FHelperEditorModeEdModeToolkit::FHelperEditorModeEdModeToolkit() {}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}
	};
	
	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(10.f)
		//.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.AutoHeight()
				[
					SNew(SBorder)
						.HAlign(HAlign_Fill)
						.Padding(5.f)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Selection color:"))
						]
				]
			+ SVerticalBox::Slot()
				.Padding(0.f, 1.f, 0.f, 0.f)
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SBorder)
						.HAlign(HAlign_Fill)
						.Padding(5.f)
						.Content()
						[
							SNew(SColorPicker).
								DisplayInlineVersion(true).
								OnColorCommitted_Lambda([this](FLinearColor Color)
								{
									TIsDerivedFrom<>
									if (auto CurrentEditorMode = StaticCast<FHelperEditorModeEdMode*>(GetEditorMode()))
									{
										CurrentEditorMode->ChangeSelectionColor(Color);
										
									}
								})
						]
				]
		];
		
	FModeToolkit::Init(InitToolkitHost);
	return;
}

// --------------------------------------------------------------------------------------
FName FHelperEditorModeEdModeToolkit::GetToolkitFName() const
{
	return FName("HelperEditorModeEdMode");
}

// --------------------------------------------------------------------------------------
FText FHelperEditorModeEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("HelperEditorModeEdModeToolkit", "DisplayName", "HelperEditorModeEdMode Tool");
}

// --------------------------------------------------------------------------------------
class FEdMode* FHelperEditorModeEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FHelperEditorModeEdMode::EM_HelperEditorModeEdModeId);
}

#undef LOCTEXT_NAMESPACE
