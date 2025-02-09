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
#include <Source/SaveGame/PlayerCoreSaveGame.h>
#include "GameFramework/GameMode.h"
#include <Source/SaveGame/GameTimeSaveGame.h>
#include <Source/SaveGame/CraftingSaveGame.h>
#include "Engine/DataTable.h"
#include <Source/Factory/ItemFactory.h>

void USaveGameSlot::SetupNewSlot(const TSubclassOf<UCraftingSaveGame>& InCraftingSaveGameClass, const TArray<TObjectPtr<UDataTable>>& InCraftingDataTables)
{	
	PlayerCoreSaveGame = NewObject<UPlayerCoreSaveGame>(this);
	PlayerInventorySaveGame = NewObject<UPlayerInventorySaveGame>(this);
	PlayerStatsSaveGame = NewObject<UPlayerStatsSaveGame>(this);
	GameTimeSaveGame = NewObject<UGameTimeSaveGame>(this);

	CraftingSaveGameClass = InCraftingSaveGameClass;
	CraftingDataTables = InCraftingDataTables;
	CraftingSaveGame = NewObject<UCraftingSaveGame>(this, CraftingSaveGameClass);
}

void USaveGameSlot::ProcessSaveSlot(const UObject& WorldContextObject)
{
	CHECK(PlayerCoreSaveGame);
	CHECK(PlayerInventorySaveGame);
	CHECK(PlayerStatsSaveGame);
	CHECK(GameTimeSaveGame);
	CHECK(CraftingSaveGame);

	TObjectPtr<UWorld> pWorld = WorldContextObject.GetWorld();
	CHECK(pWorld);

	TObjectPtr<APlayerCharacter> pCharacter = Cast<APlayerCharacter>(pWorld->GetFirstPlayerController()->GetPawn());
	CHECK(pCharacter);

	// Save player core
	PlayerCoreSaveGame->OnSave(WorldContextObject, *pCharacter);

	// Get the player inventory instance
	TObjectPtr<UPlayerInventoryComponent> pPlayerInventoryComp = pCharacter->GetComponentByClass<UPlayerInventoryComponent>();
	CHECK(pPlayerInventoryComp);

	PlayerInventorySaveGame->OnSave(WorldContextObject, *pPlayerInventoryComp);

	// Get objects that have an inventory component (not player), add the FGuid of the object
	 
	// 1. Chests
	TArray<TObjectPtr<AChest>> chestArr = UCommonUtils::GetAllActorsInWorld<AChest>(*pWorld);

	for (auto pChest : chestArr)
	{
		if (pChest) 
		{
			FGuid objUniqueId = pChest->GetCustomUniqueId();
			TObjectPtr<UInventorySaveGame> pInvSaveGame = NewObject<UInventorySaveGame>(this);
			CHECK(pInvSaveGame);
			pInvSaveGame->OnSave(WorldContextObject, *pChest->GetComponentByClass<UInventoryComponent>());
			InventorySaveGames.Add(objUniqueId, pInvSaveGame);
		}
	}

	// 2. Planters
	TArray<TObjectPtr<APlanter>> planterArr = UCommonUtils::GetAllActorsInWorld<APlanter>(*pWorld);

	for (auto pPlanter : planterArr)
	{
		if (pPlanter)
		{
			FGuid objUniqueId = pPlanter->GetCustomUniqueId();
			TObjectPtr<UInventorySaveGame> pInvSaveGame = NewObject<UInventorySaveGame>(this);
			CHECK(pInvSaveGame);
			pInvSaveGame->OnSave(WorldContextObject, *pPlanter->GetComponentByClass<UInventoryComponent>());
			InventorySaveGames.Add(objUniqueId, pInvSaveGame);
		}
	}
	
	// Player Stats
	TObjectPtr<UStatsComponent> pStatsComponent = pCharacter->GetComponentByClass<UStatsComponent>();
	CHECK(pStatsComponent);

	PlayerStatsSaveGame->OnSave(WorldContextObject, *pStatsComponent);

	// Items
	TArray<TObjectPtr<AItemSpawner>> itemArr = UCommonUtils::GetAllActorsInWorld<AItemSpawner>(*pWorld);

	for (auto pItemSpawner : itemArr)
	{
		if (pItemSpawner)
		{
			FGuid objUniqueId = pItemSpawner->GetCustomUniqueId();
			TObjectPtr<UItemSpawnerSaveGame> pItemSaveGame = NewObject<UItemSpawnerSaveGame>(this);
			CHECK(pItemSaveGame);
			pItemSaveGame->OnSave(WorldContextObject, *pItemSpawner);
			ItemSpawnerSaveGames.Add(objUniqueId, pItemSaveGame);
		}
	}

	// GameTime
	TObjectPtr<AGameModeBase> pGameMode = UGameplayStatics::GetGameMode(pWorld);
	CHECK(pGameMode);

	TObjectPtr<UGameTimeManager> pGameTimeManager = pGameMode->FindComponentByClass<UGameTimeManager>();
	CHECK(pGameTimeManager);

	GameTimeSaveGame->OnSave(WorldContextObject, *pGameTimeManager);

	// Crafting - No action since we are using the SaveGame Instance directly


}

void USaveGameSlot::ProcessLoadSlot(const UObject& WorldContextObject)
{
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

			if (InventorySaveGames.Contains(objUniqueId))
			{
				TObjectPtr<UInventorySaveGame> pInvSaveGame = InventorySaveGames[objUniqueId];
				CHECK(pInvSaveGame);

				pInvSaveGame->OnLoad(WorldContextObject, *pChest->GetComponentByClass<UInventoryComponent>());
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

			if (InventorySaveGames.Contains(objUniqueId))
			{
				TObjectPtr<UInventorySaveGame> pInvSaveGame = InventorySaveGames[objUniqueId];
				CHECK(pInvSaveGame);

				pInvSaveGame->OnLoad(WorldContextObject, *pPlanter->GetComponentByClass<UInventoryComponent>());
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

			if (ItemSpawnerSaveGames.Contains(objUniqueId))
			{
				foundItems.Add(objUniqueId);

				TObjectPtr<UItemSpawnerSaveGame> pItemSaveGame = ItemSpawnerSaveGames[objUniqueId];
				CHECK(pItemSaveGame);

				pItemSaveGame->OnSave(WorldContextObject, *pItemSpawner);
			}
		}
	}

	// Items to respawn - not by default, placed by player
	for (const TPair<FGuid, TObjectPtr<UItemSpawnerSaveGame>>& kvp : ItemSpawnerSaveGames)
	{
		if (!foundItems.Contains(kvp.Key))
		{
			// Respawn the item deferred
			// Set the values
			// Complete spawn
			if (kvp.Value)
			{
				TObjectPtr<AItemSpawner> pItem = UItemFactory::SpawnDeferredItem(*pWorld, kvp.Value->ActorTransform, kvp.Value->SpawnSettings.ItemClassToSpawn);

				if (pItem)
				{
					pItem->CustomUniqueId = kvp.Key;
					kvp.Value->OnLoad(*pWorld, *pItem);
					UGameplayStatics::FinishSpawningActor(pItem, kvp.Value->ActorTransform);
				}
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
