// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Framework/Commands/Commands.h>
#include "HelperEditorWindowStyle.h"

class FHelperEditorWindowCommands : public TCommands<FHelperEditorWindowCommands>
{
public:
	FHelperEditorWindowCommands()
		: TCommands<FHelperEditorWindowCommands>(
			TEXT("HelperEditorWindow"),
			NSLOCTEXT("Contexts", "HelperEditorWindow", "HelperEditorWindow Module"),
			NAME_None,
			FHelperEditorWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};