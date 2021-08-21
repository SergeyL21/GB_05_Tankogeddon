// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#define DEBUG_MESSAGE(Id, Color, Text, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage( \
			Id, \
			0.f, \
			Color, \
			FString::Printf(TEXT(Text), __VA_ARGS__) \
		); \
	}
