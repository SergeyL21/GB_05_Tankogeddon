// Fill out your copyright notice in the Description page of Project Settings.


#include "SMiniMap.h"

#include <Rendering/DrawElements.h>
#include <Styling/CoreStyle.h>
#include <SlateOptMacros.h>

// --------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMiniMap::Construct(const FArguments& InArgs)
{
	PlayerImage = InArgs._PlayerImage;

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
	FSlateBrush Brush;
	Brush.ImageSize = LocalSize;
	//FSlateBrush Brush;
	//Brush.SetResourceObject(PlayerImage);

	// draw level border
	TArray<FVector2D> Border;
	Border.Add(LocalSize * FVector2D{ 0.f, 0.f });
	Border.Add(LocalSize * FVector2D{ 0.f, 1.f });
	Border.Add(LocalSize * FVector2D{ 1.f, 1.f });
	Border.Add(LocalSize * FVector2D{ 1.f, 0.f });
	Border.Add(LocalSize * FVector2D{ 0.f, 0.f });
	FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Border);

	// draw level obstacles
	TArray<FBox2D> Obstacles;
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.3f, 0.6f}, LocalSize * FVector2D{0.6f, 0.3f} });
	Obstacles.Add(FBox2D{ LocalSize * FVector2D{0.6f, 0.3f}, LocalSize * FVector2D{0.6f, 0.6f} });
	for (auto& Obstacle : Obstacles)
	{
		TArray<FVector2D> Points;
		Points.Add(Obstacle.Min);
		Points.Add(Obstacle.Max);

		//FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), Points);
	}

	FSlateDrawElement::MakeBox(OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(FVector2D(0.5f, 0.5f) /** Brush.ImageSize*/, Brush.ImageSize),
		&Brush,
		ESlateDrawEffect::None,
		FLinearColor{1.f, 1.f, 1.f, 0.33f}
	);

	return 0;
}
