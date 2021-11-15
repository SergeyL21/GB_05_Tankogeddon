// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperEditorMode.h"
#include "HelperEditorModeEdMode.h"

#define LOCTEXT_NAMESPACE "FHelperEditorModeModule"

// --------------------------------------------------------------------------------------
void FHelperEditorModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FHelperEditorModeEdMode>(
		FHelperEditorModeEdMode::EM_HelperEditorModeEdModeId,
		LOCTEXT("HelperEditorModeEdModeName", "HelperEditorModeEdMode"),
		FSlateIcon(),
		true
	);

	return;
}

// --------------------------------------------------------------------------------------
void FHelperEditorModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FHelperEditorModeEdMode::EM_HelperEditorModeEdModeId);

	return;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHelperEditorModeModule, HelperEditorMode)