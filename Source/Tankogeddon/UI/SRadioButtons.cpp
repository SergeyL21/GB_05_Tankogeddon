// Fill out your copyright notice in the Description page of Project Settings.


#include "SRadioButtons.h"
#include <SlateOptMacros.h>

// --------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRadioButtons::Construct(const FArguments& InArgs)
{
	OnRadioButtonChanged = InArgs._OnRadioButtonChanged;
	SetRadioButtonStyle(InArgs._RadioButtonsStyleArg);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_1, TEXT("Easy"))
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_2, TEXT("Normal"))
		]
		+ SVerticalBox::Slot()
		[
			CreateRadioButton(ERadioButtonId::Radio_3, TEXT("Hard"))
		]
	];

	return;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

// --------------------------------------------------------------------------------------
void SRadioButtons::SetRadioButtonStyle(const FRadioButtonsStyle* InStyle)
{
	CheckBoxStyle = &InStyle->CheckBoxStyle;
	TextStyle = &InStyle->TextStyle;

	return;
}

// --------------------------------------------------------------------------------------
ECheckBoxState SRadioButtons::IsChecked(ERadioButtonId ButtonId) const
{
	return CurrentRadioButton == ButtonId ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// --------------------------------------------------------------------------------------
void SRadioButtons::OnCheckBoxChanged(ECheckBoxState NewState, ERadioButtonId ButtonId)
{
	if (NewState == ECheckBoxState::Checked)
	{
		CurrentRadioButton = ButtonId;
		OnRadioButtonChanged.ExecuteIfBound(ButtonId);
	}

	return;
}

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> SRadioButtons::CreateRadioButton(ERadioButtonId ButtonId, const FString& Text)
{
	return SNew(SCheckBox)
		.IsChecked(MakeAttributeRaw(this, &SRadioButtons::IsChecked, ButtonId))
		.OnCheckStateChanged(this, &SRadioButtons::OnCheckBoxChanged, ButtonId)
		.Style(CheckBoxStyle)
		[
			SNew(STextBlock)
			.TextStyle(TextStyle)
			.Text(FText::FromString(Text))
		];
}