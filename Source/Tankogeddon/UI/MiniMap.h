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
    UPROPERTY(EditAnywhere, Category = "Appearance"/*, meta = (MakeEditWidget = true)*/)
    UTexture2D* PlayerImage;

public:
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

    void SetPlayerPercentPos(const FVector& WorldLocation, const FVector2D& WorldSize);
    FVector2D GetPlayerPercentPos() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
    TSharedPtr<SMiniMap> MyMiniMap;

private:
    FVector2D PlayerPercentPos;
};
