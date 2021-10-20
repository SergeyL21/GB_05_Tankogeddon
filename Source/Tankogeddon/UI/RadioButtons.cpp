// Fill out your copyright notice in the Description page of Project Settings.

#include "RadioButtons.h"

// --------------------------------------------------------------------------------------
void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    RadioButtonsHolder.Reset();

    return;
}

// --------------------------------------------------------------------------------------
void URadioButtons::OnRadioButtonChangedFunc(ERadioButtonId ButtonId)
{
    OnRadioButtonChanged.Broadcast(ButtonId);

    return;
}

// --------------------------------------------------------------------------------------
TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
    RadioButtonsHolder = SNew(SRadioButtons)
        .RadioButtonsStyleArg(&WidgetStyle)
        .OnRadioButtonChanged_UObject(this, &URadioButtons::OnRadioButtonChangedFunc);

    return RadioButtonsHolder.ToSharedRef();
}
