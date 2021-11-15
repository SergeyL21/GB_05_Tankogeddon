// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <EdMode.h>

class FHelperEditorModeEdMode : public FEdMode
{
protected:
	TArray<AActor*> SelectedActors;

public:
	const static FEditorModeID EM_HelperEditorModeEdModeId;
	
public:
	FHelperEditorModeEdMode();
	virtual ~FHelperEditorModeEdMode() override;

	// FEdMode interface
	virtual void Enter() override;
	
	virtual void Exit() override;
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport,
		const FSceneView* View, FCanvas* Canvas) override;
	
	virtual void ActorSelectionChangeNotify() override;
	
	virtual bool UsesToolkits() const override;
	// End of FEdMode interface

	void ChangeSelectionColor(const FLinearColor &Color);

protected:
	void UpdateSelectedActors();
	
	void UpdateQuestSystemPawn(const class AQuestSystemPawn *QuestSystemPawn);

private:
	FLinearColor SelectionColor;
};
