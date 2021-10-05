// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/Widget.h>
#include "SRadioButtons.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonChangedEvent, ERadioButtonId, NewRadioButtonId);

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API URadioButtons : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnRadioButtonChangedEvent OnRadioButtonChanged;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FRadioButtonsStyle WidgetStyle;

public:
	virtual void ReleaseSlateResources(bool bRelealeseChildren) override;

protected:

	void OnRadioButtonChangedFunc(ERadioButtonId ButtonId);

	virtual TSharedRef<SWidget> RebuildWidget() override;

	TSharedPtr<SRadioButtons> RadioButtonsHolder;
};
