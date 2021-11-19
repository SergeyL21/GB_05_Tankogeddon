// Fill out your copyright notice in the Description page of Project Settings.

#include "TankogeddonGameInstance.h"
#include "Saving/SavingsManager.h"

// --------------------------------------------------------------------------------------
void UTankogeddonGameInstance::Init()
{
	Super::Init();

	SaveManager = NewObject<USavingsManager>(this);
	SaveManager->Init();
}

