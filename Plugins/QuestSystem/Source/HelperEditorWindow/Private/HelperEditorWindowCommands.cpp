// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperEditorWindowCommands.h"

#define LOCTEXT_NAMESPACE "FHelperEditorWindowModule"

// --------------------------------------------------------------------------------------
void FHelperEditorWindowCommands::RegisterCommands()
{
	UI_COMMAND(
		OpenPluginWindow,
		"HelperEditorWindow",
		"Bring up HelperEditorWindow window",
		EUserInterfaceActionType::Button,
		FInputGesture()
	);

	return;
}

#undef LOCTEXT_NAMESPACE
