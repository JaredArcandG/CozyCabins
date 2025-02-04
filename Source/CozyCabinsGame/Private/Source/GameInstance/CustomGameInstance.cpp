// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/GameInstance/CustomGameInstance.h"
#include "Source/SaveGame/CraftingSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UCustomGameInstance::Init()
{
	// If multiple SaveGame Slots are supported, this should be called when selecting a Save instead and use said Index
	InitializeSaveGames();
}

void UCustomGameInstance::InitializeSaveGames()
{
	// Check if Crafting SaveGame exists already, otherwise create one
	if (UGameplayStatics::DoesSaveGameExist(CraftingSaveGame_SlotName, SaveGameSlotIndex))
	{
		CraftingSaveGame = Cast<UCraftingSaveGame>(UGameplayStatics::LoadGameFromSlot(CraftingSaveGame_SlotName, SaveGameSlotIndex));
	}
	else 
	{
		CraftingSaveGame = Cast<UCraftingSaveGame>(UGameplayStatics::CreateSaveGameObject(CraftingSaveGameClass));
		SaveCraftingSaveGame();
	}
}

void UCustomGameInstance::SaveCraftingSaveGame()
{
	if (CraftingSaveGame) 
	{
		UGameplayStatics::SaveGameToSlot(CraftingSaveGame, CraftingSaveGame_SlotName, SaveGameSlotIndex);
	}
}

