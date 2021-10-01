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
void UMiniMap::SetPlayerPercentPos(const FVector& WorldLocation, const FVector2D& WorldSize)
{
    PlayerPercentPos.X = 0.5f + (WorldLocation.Y / WorldSize.Y);
    PlayerPercentPos.Y = 0.5f - (WorldLocation.X / WorldSize.X);

    return;
}

// --------------------------------------------------------------------------------------
FVector2D UMiniMap::GetPlayerPercentPos() const
{
    return PlayerPercentPos;
}

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> UMiniMap::RebuildWidget()
{
    MyMiniMap = SNew(SMiniMap)
        .PlayerImageArg(this->PlayerImage)
        .PlayerPercentPosArg(
            TAttribute<FVector2D>::Create(
                TAttribute<FVector2D>::FGetter::CreateUObject(this, &UMiniMap::GetPlayerPercentPos)));

    return MyMiniMap.ToSharedRef();
}
