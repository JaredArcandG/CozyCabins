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

}

void UCustomGameInstance::InitializeSaveGameSlots(UWorld* World)
{
	for (int slotIdx = 0; slotIdx < NumMaxGameSlots; slotIdx++)
	{

		GameSlots[slotIdx] = NewObject<USaveGameSlot>(this);
		CHECK(GameSlots[slotIdx]);

		GameSlots[slotIdx]->Setup(SaveGameSlotNameBase, slotIdx, CraftingSaveGameClass, CraftingDataTables);
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

	CHECK(GameSlots[ChosenSlotIdx]);

	// Process the game slot (Store all required elements)
	GameSlots[ChosenSlotIdx]->ProcessSaveSlot(*this);

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

