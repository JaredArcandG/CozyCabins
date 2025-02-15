// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/SaveGameSlot.h"
#include "Kismet/GameplayStatics.h"
#include <Source/Utils/CheckUtils.h>
#include "Source/SaveGame/PlayerInventorySaveGame.h"
#include "GameFramework/PlayerController.h"
#include <Source/Player/PlayerCharacter.h>
#include "Source/Components/PlayerInventoryComponent.h"
#include <Source/Utils/CommonUtils.h>
#include <Source/Storage/Chest.h>
#include <Source/SaveGame/InventorySaveGame.h>
#include <Source/Farming/Planter.h>
#include <Source/SaveGame/PlayerStatsSaveGame.h>
#include <Source/Items/ItemSpawner.h>
#include <Source/SaveGame/ItemSpawnerSaveGame.h>
#include "GameFramework/GameMode.h"
#include <Source/SaveGame/GameTimeSaveGame.h>
#include <Source/SaveGame/CraftingSaveGame.h>
#include "Engine/DataTable.h"
#include <Source/Factory/ItemFactory.h>
#include <Source/SaveGame/PlayerCoreSaveGame.h>

void USaveGameSlot::Setup(const FString& InBaseSaveName, const int& InSaveIndex, const TSubclassOf<UCraftingSaveGame>& InCraftingSaveGameClass, const TArray<TObjectPtr<UDataTable>>& InCraftingDataTables)
{	
	BaseSaveName = InBaseSaveName + FString::FromInt(InSaveIndex);
	SaveIndex = InSaveIndex;

	PlayerCoreSaveGameExtString = BaseSaveName + FString("_PlayerCore");
	PlayerCoreSaveGame = PreProcessSaveGame<UPlayerCoreSaveGame>(PlayerCoreSaveGameExtString, SaveIndex);

	PlayerInventorySaveGameExtString = BaseSaveName + FString("_PlayerInventory");
	PlayerInventorySaveGame = PreProcessSaveGame<UPlayerInventorySaveGame>(PlayerInventorySaveGameExtString, SaveIndex);

	InventorySaveGameExtString = BaseSaveName + FString("_Inventory");
	InventorySaveGame = PreProcessSaveGame<UInventorySaveGame>(InventorySaveGameExtString, SaveIndex);

	ItemSpawnerSaveGamesExtString = BaseSaveName + FString("_ItemSpawner");
	ItemSpawnerSaveGame = PreProcessSaveGame<UItemSpawnerSaveGame>(ItemSpawnerSaveGamesExtString, SaveIndex);

	PlayerStatsSaveGameExtString = BaseSaveName + FString("_PlayerStats");
	PlayerStatsSaveGame = PreProcessSaveGame<UPlayerStatsSaveGame>(PlayerStatsSaveGameExtString, SaveIndex);

	GameTimeSaveGameExtString = BaseSaveName + FString("_GameTime");
	GameTimeSaveGame = PreProcessSaveGame<UGameTimeSaveGame>(GameTimeSaveGameExtString, SaveIndex);

	CraftingSaveGameClass = InCraftingSaveGameClass;
	CraftingDataTables = InCraftingDataTables;
	CraftingSaveGameExtString = BaseSaveName + FString("_Crafting");
	CraftingSaveGame = PreProcessSaveGame(CraftingSaveGameClass, CraftingSaveGameExtString, SaveIndex);

}

void USaveGameSlot::ProcessSaveSlot(const UObject& WorldContextObject)
{
	CHECK(PlayerCoreSaveGame);
	CHECK(PlayerInventorySaveGame);
	CHECK(InventorySaveGame);
	CHECK(ItemSpawnerSaveGame);
	CHECK(PlayerStatsSaveGame);
	CHECK(GameTimeSaveGame);
	CHECK(CraftingSaveGame);

	// Clear data for overwrite in case it's a new save
	PlayerCoreSaveGame->ClearOverwrite();
	PlayerInventorySaveGame->ClearOverwrite();
	InventorySaveGame->ClearOverwrite();
	PlayerStatsSaveGame->ClearOverwrite();
	GameTimeSaveGame->ClearOverwrite();
	ItemSpawnerSaveGame->ClearOverwrite();

	TObjectPtr<UWorld> pWorld = WorldContextObject.GetWorld();
	CHECK(pWorld);

	TObjectPtr<APlayerCharacter> pCharacter = Cast<APlayerCharacter>(pWorld->GetFirstPlayerController()->GetPawn());
	CHECK(pCharacter);

	// Save player core
	PlayerCoreSaveGame->OnSave(WorldContextObject, *pCharacter);
	UGameplayStatics::SaveGameToSlot(PlayerCoreSaveGame, PlayerCoreSaveGameExtString, SaveIndex);

	// Get the player inventory instance
	TObjectPtr<UPlayerInventoryComponent> pPlayerInventoryComp = pCharacter->GetComponentByClass<UPlayerInventoryComponent>();
	CHECK(pPlayerInventoryComp);

	PlayerInventorySaveGame->OnSave(WorldContextObject, *pPlayerInventoryComp);
	UGameplayStatics::SaveGameToSlot(PlayerInventorySaveGame, PlayerInventorySaveGameExtString, SaveIndex);

	// Get objects that have an inventory component (not player), add the FGuid of the object

	// 1. Chests
	TArray<TObjectPtr<AChest>> chestArr = UCommonUtils::GetAllActorsInWorld<AChest>(*pWorld);

	for (auto pChest : chestArr)
	{
		if (pChest) 
		{
			FGuid objUniqueId = pChest->GetCustomUniqueId();
			InventorySaveGame->OnSaveUnique(WorldContextObject, objUniqueId, *pChest->GetComponentByClass<UInventoryComponent>());
		}
	}

	// 2. Planters
	TArray<TObjectPtr<APlanter>> planterArr = UCommonUtils::GetAllActorsInWorld<APlanter>(*pWorld);

	for (auto pPlanter : planterArr)
	{
		if (pPlanter)
		{
			FGuid objUniqueId = pPlanter->GetCustomUniqueId();
			InventorySaveGame->OnSaveUnique(WorldContextObject, objUniqueId, *pPlanter->GetComponentByClass<UInventoryComponent>());
		}
	}

	UGameplayStatics::SaveGameToSlot(InventorySaveGame, InventorySaveGameExtString, SaveIndex);

	// Player Stats
	TObjectPtr<UStatsComponent> pStatsComponent = pCharacter->GetComponentByClass<UStatsComponent>();
	CHECK(pStatsComponent);

	PlayerStatsSaveGame->OnSave(WorldContextObject, *pStatsComponent);
	UGameplayStatics::SaveGameToSlot(PlayerStatsSaveGame, PlayerStatsSaveGameExtString, SaveIndex);

	// Items
	TArray<TObjectPtr<AItemSpawner>> itemArr = UCommonUtils::GetAllActorsInWorld<AItemSpawner>(*pWorld);

	for (auto pItemSpawner : itemArr)
	{
		if (pItemSpawner)
		{
			FGuid objUniqueId = pItemSpawner->GetCustomUniqueId();
			ItemSpawnerSaveGame->OnSaveUnique(WorldContextObject, objUniqueId, *pItemSpawner);
		}
	}

	UGameplayStatics::SaveGameToSlot(ItemSpawnerSaveGame, ItemSpawnerSaveGamesExtString, SaveIndex);

	// GameTime
	TObjectPtr<AGameModeBase> pGameMode = UGameplayStatics::GetGameMode(pWorld);
	CHECK(pGameMode);

	TObjectPtr<UGameTimeManager> pGameTimeManager = pGameMode->FindComponentByClass<UGameTimeManager>();
	CHECK(pGameTimeManager);

	GameTimeSaveGame->OnSave(WorldContextObject, *pGameTimeManager);

	UGameplayStatics::SaveGameToSlot(GameTimeSaveGame, GameTimeSaveGameExtString, SaveIndex);

	// Crafting - No action since we are using the SaveGame Instance directly
	UGameplayStatics::SaveGameToSlot(CraftingSaveGame, CraftingSaveGameExtString, SaveIndex);

}

void USaveGameSlot::ProcessLoadSlot(const UObject& WorldContextObject)
{
	CHECK(PlayerCoreSaveGame);
	CHECK(PlayerInventorySaveGame);
	CHECK(InventorySaveGame);
	CHECK(ItemSpawnerSaveGame);
	CHECK(PlayerStatsSaveGame);
	CHECK(GameTimeSaveGame);
	CHECK(CraftingSaveGame);

	TObjectPtr<UWorld> pWorld = WorldContextObject.GetWorld();
	CHECK(pWorld);

	TObjectPtr<APlayerCharacter> pCharacter = Cast<APlayerCharacter>(pWorld->GetFirstPlayerController()->GetPawn());
	CHECK(pCharacter);

	// Load player core
	PlayerCoreSaveGame->OnLoad(WorldContextObject, *pCharacter);

	// Get and load the player inventory instance
	TObjectPtr<UPlayerInventoryComponent> pPlayerInventoryComp = pCharacter->GetComponentByClass<UPlayerInventoryComponent>();
	CHECK(pPlayerInventoryComp);

	PlayerInventorySaveGame->OnLoad(WorldContextObject, *pPlayerInventoryComp);

	// 1. Load Chests
	TArray<TObjectPtr<AChest>> chestArr = UCommonUtils::GetAllActorsInWorld<AChest>(*pWorld);

	for (auto pChest : chestArr)
	{
		if (pChest)
		{
			FGuid objUniqueId = pChest->GetCustomUniqueId();

			// The chest exists
			if (InventorySaveGame->DataMap.Contains(objUniqueId))
			{
				if (pChest->GetComponentByClass<UInventoryComponent>())
				{
					InventorySaveGame->OnLoadUnique(WorldContextObject, objUniqueId, *pChest->GetComponentByClass<UInventoryComponent>());
				}
			} 
		}
	}

	// 2. Load Planters
	TArray<TObjectPtr<APlanter>> planterArr = UCommonUtils::GetAllActorsInWorld<APlanter>(*pWorld);

	for (auto pPlanter : planterArr)
	{
		if (pPlanter)
		{
			FGuid objUniqueId = pPlanter->GetCustomUniqueId();

			if (InventorySaveGame->DataMap.Contains(objUniqueId))
			{
				if (pPlanter->GetComponentByClass<UInventoryComponent>())
				{
					InventorySaveGame->OnLoadUnique(WorldContextObject, objUniqueId, *pPlanter->GetComponentByClass<UInventoryComponent>());
				}
			}
		}
	}

	// Load Player Stats
	TObjectPtr<UStatsComponent> pStatsComponent = pCharacter->GetComponentByClass<UStatsComponent>();
	CHECK(pStatsComponent);

	PlayerStatsSaveGame->OnLoad(WorldContextObject, *pStatsComponent);

	// Items - Existing
	TArray<TObjectPtr<AItemSpawner>> itemArr = UCommonUtils::GetAllActorsInWorld<AItemSpawner>(*pWorld);
	TArray<FGuid> foundItems;

	for (auto pItemSpawner : itemArr)
	{
		if (pItemSpawner)
		{
			FGuid objUniqueId = pItemSpawner->GetCustomUniqueId();

			if (ItemSpawnerSaveGame->DataMap.Contains(objUniqueId))
			{
				foundItems.Add(objUniqueId);

				ItemSpawnerSaveGame->OnLoadUnique(WorldContextObject, objUniqueId, *pItemSpawner);
			}
		}
	}

	// Items to respawn - not by default, placed by player
	for (const TPair<FGuid, FItemSpawnerSaveGameData>& kvp : ItemSpawnerSaveGame->DataMap)
	{
		if (!foundItems.Contains(kvp.Key))
		{
			// Respawn the item deferred
			// Set the values
			// Complete spawn
			TObjectPtr<AItemSpawner> pItem = UItemFactory::SpawnDeferredItem(*pWorld, kvp.Value.ActorTransform, kvp.Value.SpawnSettings.ItemClassToSpawn);

			if (pItem)
			{
				pItem->CustomUniqueId = kvp.Key;
				ItemSpawnerSaveGame->OnLoadUnique(WorldContextObject, kvp.Key, *pItem);
				UGameplayStatics::FinishSpawningActor(pItem, kvp.Value.ActorTransform);
			}
		}
	}

	// GameTime
	TObjectPtr<AGameModeBase> pGameMode = UGameplayStatics::GetGameMode(pWorld);
	CHECK(pGameMode);

	TObjectPtr<UGameTimeManager> pGameTimeManager = pGameMode->FindComponentByClass<UGameTimeManager>();
	CHECK(pGameTimeManager);

	GameTimeSaveGame->OnLoad(WorldContextObject, *pGameTimeManager);

	// Crafting - No action since we are using the SaveGame Instance directly
}

FString USaveGameSlot::GetBaseSaveName()
{
	return BaseSaveName;
}

/// <summary>
/// Ensure all game save slots are preprocessed correctly 
/// Ensures saving and loading the slot works as expected
/// </summary>
/// <param name="OutSaveGame"></param>
/// <param name="SaveGameClass"></param>
/// <param name="InSlotExt"></param>
/// <param name="InSlotIndex"></param>
template <typename T>
T* USaveGameSlot::PreProcessSaveGame(const FString& InSlotExt, const int& InSlotIndex)
{
	// If the slot already exists, load the savegame from that slot
	if (UGameplayStatics::DoesSaveGameExist(InSlotExt, InSlotIndex))
	{
		return Cast<T>(UGameplayStatics::LoadGameFromSlot(InSlotExt, InSlotIndex));
	}
	else // Otherwise, create a new savegame
	{
		return Cast<T>(UGameplayStatics::CreateSaveGameObject(T::StaticClass()));
	}
}

/// <summary>
/// Ensure all game save slots are preprocessed correctly
/// Ensures saving and loading the slot works as expected
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="SaveGameClass"></param>
/// <param name="InSlotExt"></param>
/// <param name="InSlotIndex"></param>
/// <returns></returns>
template <typename T>
T* USaveGameSlot::PreProcessSaveGame(const TSubclassOf<T>& SaveGameClass, const FString& InSlotExt, const int32 InSlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(InSlotExt, InSlotIndex))
	{
		return Cast<T>(UGameplayStatics::LoadGameFromSlot(InSlotExt, InSlotIndex));
	}
	else
	{
		return Cast<T>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
}

