// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

/**
 * 
 */
class TANKOGEDDON_API SMiniMap : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMiniMap)
	{}

	SLATE_ARGUMENT(UTexture2D*, PlayerImageArg)

	SLATE_ATTRIBUTE(FVector2D, PlayerPercentPosArg)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

protected:
	UTexture2D* PlayerImage;
	
	TAttribute<FVector2D> PlayerPercentPos;
};
