// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#define DEBUG_MESSAGE(Id, Color, Text, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage( \
			Id, \
			3.f, \
			Color, \
			FString::Printf(TEXT(Text), __VA_ARGS__) \
		); \
	}

#define DEBUG_MESSAGE_EX(Id, Color, Text, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage( \
			Id, \
			3.f, \
			Color, \
			FString::Printf(TEXT(Text), __VA_ARGS__) \
		); \
	}
