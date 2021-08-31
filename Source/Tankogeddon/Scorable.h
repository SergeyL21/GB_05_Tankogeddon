// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include "Scorable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UScorable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKOGEDDON_API IScorable
{
	GENERATED_BODY()

public:
	//virtual ~IScorable() = default;

	virtual int32 GetScorePoints() const;
};
