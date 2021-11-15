// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperEditorModeEdMode.h"
#include "HelperEditorModeEdModeToolkit.h"
#include <Toolkits/ToolkitManager.h>
#include <EditorModeManager.h>
#include <Engine/Selection.h>
#include <Kismet/GameplayStatics.h>

#include "Objective.h"
#include "Quest.h"
#include "QuestSystemPawn.h"

#include <functional>

// --------------------------------------------------------------------------------------
const FEditorModeID FHelperEditorModeEdMode::EM_HelperEditorModeEdModeId = TEXT("EM_HelperEditorModeEdMode");

// --------------------------------------------------------------------------------------
FHelperEditorModeEdMode::FHelperEditorModeEdMode()
{
	SelectionColor = FColor::Blue;
}

// --------------------------------------------------------------------------------------
FHelperEditorModeEdMode::~FHelperEditorModeEdMode() {}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FHelperEditorModeEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	UpdateSelectedActors();

	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();

	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	for (auto BoundedActor : SelectedActors)
	{
		DrawWireSphere(
			PDI,
			BoundedActor->GetActorLocation(),
			SelectionColor,
			100.f,
			32,
			1
		);
	}

	FEdMode::Render(View, Viewport, PDI);
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport,
	const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	FString HUDText;
	for (auto SelectedActor : SelectedActors)
	{
		HUDText = Cast<AQuest>(SelectedActor) ?
			FString::Printf(TEXT("[QUEST] %s"), *SelectedActor->GetName()) :
			FString::Printf(TEXT("[OBJECTIVE] %s"), *SelectedActor->GetName());
		
		if (Canvas)
		{
			FBox Bounds {SelectedActor->GetComponentsBoundingBox(true)};

			FVector drawPos {Bounds.GetCenter() + FVector(0.f, 0.f, Bounds.GetExtent().Z)};
			FVector2D PixelLocation;
			View->ScreenToPixel(View->WorldToScreen(drawPos), PixelLocation);
			PixelLocation /= ViewportClient->GetDPIScale();
			auto RenderFont {GEngine->GetSmallFont()};
			Canvas->DrawShadowedText(
				PixelLocation.X,
				PixelLocation.Y, 
				FText::FromString(HUDText), 
				RenderFont,
				SelectionColor
			);
		}
	}
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();

	UpdateSelectedActors();
	return;
}

// --------------------------------------------------------------------------------------
bool FHelperEditorModeEdMode::UsesToolkits() const
{
	return true;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::ChangeSelectionColor(const FLinearColor& Color)
{
	SelectionColor = Color;
	UpdateSelectedActors();
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::UpdateSelectedActors()
{
	SelectedActors.Empty();

	auto ActorsSelection {GEditor->GetSelectedActors()};
	for (FSelectionIterator Iter{*ActorsSelection}; Iter; ++Iter)
	{
		if (AQuest* QuestActor = Cast<AQuest>(*Iter))
		{
			SelectedActors.AddUnique(QuestActor);
		}
		else if (AQuestSystemPawn *QuestSystemPawn = Cast<AQuestSystemPawn>(*Iter))
		{
			UpdateQuestSystemPawn(QuestSystemPawn);
		}
	}

	if (SelectedActors.Num() < 1)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuest::StaticClass(), SelectedActors);
	}
	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeEdMode::UpdateQuestSystemPawn(const AQuestSystemPawn *QuestSystemPawn)
{
	std::function<void(const AQuest*)> UpdateLocalQuest = [&](const AQuest* Quest) mutable
	{
		if (auto PrerequisiteQuest = Quest->GetPrerequisiteQuest())
		{
			UpdateLocalQuest(PrerequisiteQuest);
		}

		auto Objectives {Quest->GetObjectives()};
		for (auto Objective : Objectives)
		{
			SelectedActors.Append(Objective->GetObjectiveActors());
		}
	};
	
	TArray<AActor*> AttachedActors;
	QuestSystemPawn->GetAttachedActors(AttachedActors);
	for (auto Actor : AttachedActors)
	{
		if (auto Quest = Cast<AQuest>(Actor))
		{
			UpdateLocalQuest(Quest);
		}
	}
	return;
}




