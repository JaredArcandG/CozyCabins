// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/GameInstance/CustomGameInstance.h"
#include "Source/SaveGame/CraftingSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/SaveGame/SaveGameSlot.h>

void UCustomGameInstance::Init()
{
	// Initialize the gameslots array
	GameSlots.SetNum(NumMaxGameSlots, false);

	// Load all the save game slots for all instances
	InitializeSaveGameSlots();
}

void UCustomGameInstance::InitializeSaveGameSlots()
{
	for (int slotIdx = 0; slotIdx < NumMaxGameSlots; slotIdx++)
	{
		// Check if Crafting SaveGame exists already
		if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, slotIdx))
		{
			GameSlots[slotIdx] = Cast<USaveGameSlot>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, slotIdx));
		}
	}
}

/// <summary>
/// Called when a game save is initiated
/// </summary>
void UCustomGameInstance::OnSaveGame(const int& ChosenSlotIdx)
{
	// Invalid slot index chosen
	if (!GameSlots.IsValidIndex(ChosenSlotIdx))
	{
		return;
	}

	// Make a new game slot if it didn't exist
	if (!GameSlots[ChosenSlotIdx])
	{
		GameSlots[ChosenSlotIdx] = Cast<USaveGameSlot>(UGameplayStatics::CreateSaveGameObject(USaveGameSlot::StaticClass()));
		GameSlots[ChosenSlotIdx]->SetupNewSlot(CraftingSaveGameClass, CraftingDataTables);
	}

	// Clear the slot (in case existing slot was there)

	// Process the game slot (Store all required elements)
	GameSlots[ChosenSlotIdx]->ProcessSaveSlot(*this);

	// Save the game slot
	UGameplayStatics::SaveGameToSlot(GameSlots[ChosenSlotIdx], SaveGameSlotName, ChosenSlotIdx);
}

/// <summary>
/// Called when the game load is initiated
/// </summary>
/// <param name="ChosenSlotIdx"></param>
void UCustomGameInstance::OnLoadGame(const int& ChosenSlotIdx)
{
	// Invalid slot index chosen
	if (!GameSlots.IsValidIndex(ChosenSlotIdx))
	{
		return;
	}

	// Valid slot
	if (GameSlots[ChosenSlotIdx])
	{
		GameSlots[ChosenSlotIdx]->ProcessLoadSlot(*this);
	}
}

