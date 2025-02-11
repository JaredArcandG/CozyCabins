// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/BaseSaveGame.h"

void UBaseSaveGame::OnSave(const UObject& WorldContextObject, UObject& ObjectToSave)
{
	// Common functionality
}

void UBaseSaveGame::OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad)
{
	// Common functionality 
}

void UBaseSaveGame::OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave)
{
	// Common functionality
}

void UBaseSaveGame::OnLoadUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToLoad)
{
	// Common functionality
}

void UBaseSaveGame::ClearOverwrite()
{
	// Common functionality
}
