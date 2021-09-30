// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/Widget.h>
#include "MiniMap.generated.h"

class SMiniMap;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMiniMap : public UWidget
{
	GENERATED_BODY()
	
public:
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
    TSharedPtr<SMiniMap> MyMiniMap;
};
