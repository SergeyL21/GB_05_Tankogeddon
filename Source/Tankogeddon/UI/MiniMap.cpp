// Fill out your copyright notice in the Description page of Project Settings.

#include "MiniMap.h"
#include "SMiniMap.h"

// --------------------------------------------------------------------------------------
void UMiniMap::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    MyMiniMap.Reset();
}

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> UMiniMap::RebuildWidget()
{
    MyMiniMap = SNew(SMiniMap);

    return MyMiniMap.ToSharedRef();
}
