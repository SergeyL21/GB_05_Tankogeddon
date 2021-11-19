// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSaveGame.h"

// --------------------------------------------------------------------------------------
ULevelSaveGame::ULevelSaveGame()
{
	Player = NewObject<UPawnSaveGame>();
}
