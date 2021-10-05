// Fill out your copyright notice in the Description page of Project Settings.

#include "RadioButtons.h"

// --------------------------------------------------------------------------------------
void URadioButtons::ReleaseSlateResources(bool bRelealeseChildren)
{
    Super::ReleaseSlateResources(bRelealeseChildren);
    RadioButtonsHolder.Reset();

    return;
}

// --------------------------------------------------------------------------------------
void URadioButtons::OnRadioButtonChangedFunc(ERadioButtonId ButtonId)
{
    OnRadioButtonChanged.Broadcast(ButtonId);
}

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
    RadioButtonsHolder = SNew(SRadioButtons)
        .OnRadioButtonChanged_UObject(this, &URadioButtons::OnRadioButtonChangedFunc)
        .RadioButtonsStyleArg(&WidgetStyle);

    return RadioButtonsHolder.ToSharedRef();
}
