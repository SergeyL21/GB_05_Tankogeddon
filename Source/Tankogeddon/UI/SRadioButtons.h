// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Widgets/SCompoundWidget.h>

#include "RadioButtonsWidgetStyle.h"

UENUM(BlueprintType)
enum class ERadioButtonId : uint8
{
	Radio_1,
	Radio_2,
	Radio_3
};

DECLARE_DELEGATE_OneParam(FOnRadioButtonChanged, ERadioButtonId /* NewRadioButtonId */);

/**
 * 
 */
class TANKOGEDDON_API SRadioButtons : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButtons)
	{}

	/* Called when radio button changed */
	SLATE_EVENT(FOnRadioButtonChanged, OnRadioButtonChanged)

	SLATE_STYLE_ARGUMENT(FRadioButtonsStyle, RadioButtonsStyleArg)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	const FCheckBoxStyle* CheckBoxStyle;

	const FTextBlockStyle* TextStyle;

	ERadioButtonId CurrentRadioButton{ ERadioButtonId::Radio_1 };

	ECheckBoxState IsChecked(ERadioButtonId ButtonId) const;

	void OnCheckBoxChanged(ECheckBoxState NewState, ERadioButtonId ButtonId);

	TSharedRef<SWidget> CreateRadioButton(ERadioButtonId ButtonId, const FString& Text);

	FOnRadioButtonChanged OnRadioButtonChanged;
};
