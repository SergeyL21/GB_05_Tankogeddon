// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define DEBUG_MESSAGE(Id, Text, ...) \
	if (GEngine) { \
		GEngine->AddOnScreenDebugMessage( \
			Id, \
			0.f, \
			FColor::Yellow, \
			FString::Printf(TEXT(Text), __VA_ARGS__) \
		); \
	}

