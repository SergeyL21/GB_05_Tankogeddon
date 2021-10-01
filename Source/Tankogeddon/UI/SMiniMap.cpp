// Fill out your copyright notice in the Description page of Project Settings.


#include "SMiniMap.h"

#include <Rendering/DrawElements.h>
#include <Styling/CoreStyle.h>
#include <SlateOptMacros.h>

// --------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMiniMap::Construct(const FArguments& InArgs)
{
	PlayerImage = InArgs._PlayerImageArg;

	/*ChildSlot
	[
	];*/
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

// --------------------------------------------------------------------------------------
int32 SMiniMap::OnPaint(const FPaintArgs & Args, 
						const FGeometry & AllottedGeometry, 
						const FSlateRect & MyCullingRect, 
						FSlateWindowElementList & OutDrawElements, 
						int32 LayerId, 
						const FWidgetStyle & InWidgetStyle, 
						bool bParentEnabled) const
{
	const auto LocalSize{ AllottedGeometry.GetLocalSize() };
	FSlateBrush BackgroundBrush;
	BackgroundBrush.ImageSize = LocalSize;
	FSlateBrush PlayerBrush;
	PlayerBrush.SetResourceObject(PlayerImage);

	// draw transparent background
	FSlateDrawElement::MakeBox(OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(FVector2D(0.5f, 0.5f), BackgroundBrush.ImageSize),
		&BackgroundBrush,
		ESlateDrawEffect::None,
		FLinearColor{ 0.1f, 0.1f, 0.1f, 0.5f }
	);

	// draw level border
	TArray<FVector2D> Border;
	Border.Add(LocalSize * FVector2D{ 0.f, 0.f });
	Border.Add(LocalSize * FVector2D{ 0.f, 1.f });
	Border.Add(LocalSize * FVector2D{ 1.f, 1.f });
	Border.Add(LocalSize * FVector2D{ 1.f, 0.f });
	Border.Add(LocalSize * FVector2D{ 0.f, 0.f });
	FSlateDrawElement::MakeLines(
		OutDrawElements, 
		LayerId, AllottedGeometry.ToPaintGeometry(),
		Border,
		ESlateDrawEffect::None,
		FLinearColor::White,
		true,
		2.f
	);

	// draw level obstacles
	TArray<FBox2D> Obstacles;
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.2f, 0.45f}, LocalSize * FVector2D{0.8f, 0.55f} });
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.1f, 0.8f}, LocalSize * FVector2D{0.2f, 0.9f} });
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.8f, 0.8f}, LocalSize * FVector2D{0.9f, 0.9f} });
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.1f, 0.1f}, LocalSize * FVector2D{0.2f, 0.2f} });
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.8f, 0.1f}, LocalSize * FVector2D{0.9f, 0.2f} });
	for (const auto& Obstacle : Obstacles)
	{
		TArray<FVector2D> Points;
		Points.Add(Obstacle.Min);
		Points.Add(FVector2D{ Obstacle.Max.X, Obstacle.Min.Y });
		Points.Add(Obstacle.Max);
		Points.Add(FVector2D{ Obstacle.Min.X, Obstacle.Max.Y });
		Points.Add(Obstacle.Min);

		FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points);
	}

	// draw player icon
	FSlateDrawElement::MakeBox(OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(LocalSize * FVector2D(0.5f, 1.f) - PlayerBrush.ImageSize / 2.f, PlayerBrush.ImageSize),
		&PlayerBrush
	);

	return 0;
}
