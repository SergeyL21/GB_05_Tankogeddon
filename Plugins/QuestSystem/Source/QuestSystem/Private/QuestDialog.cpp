// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDialog.h"
#include "Quest.h"
#include "QuestDescription.h"

#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

#include <tuple>

// --------------------------------------------------------------------------------------
void UQuestDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (AcceptButton)
	{
		AcceptButton->OnReleased.AddDynamic(this, &ThisClass::OnAccepted);
	}

	if (RejectButton)
	{
		RejectButton->OnReleased.AddDynamic(this, &ThisClass::OnRejected);
	}

	auto PlayerController {UGameplayStatics::GetPlayerController(GetWorld(), 0)};
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, this);
	PlayerController->SetShowMouseCursor(true);

	return;
}

// --------------------------------------------------------------------------------------
void UQuestDialog::Init(AQuest* Quest)
{
	if (QuestDescription)
	{
		QuestDescription->Init(Quest);
	}

	return;
}

// --------------------------------------------------------------------------------------
void UQuestDialog::OnAccepted()
{
	HideDialog();
	std::ignore = OnQuestAccepted.ExecuteIfBound();
	return;
}

// --------------------------------------------------------------------------------------
void UQuestDialog::OnRejected()
{
	HideDialog();
	return;
}

// --------------------------------------------------------------------------------------
void UQuestDialog::HideDialog()
{
	auto PlayerController {UGameplayStatics::GetPlayerController(GetWorld(), 0)};
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	PlayerController->SetShowMouseCursor(true);
	RemoveFromViewport();
	std::ignore = OnDialogClosed.ExecuteIfBound();

	return;
}





