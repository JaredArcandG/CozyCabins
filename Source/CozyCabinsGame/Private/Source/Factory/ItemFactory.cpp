// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Factory/ItemFactory.h"
#include "Source/Items/Item.h"
#include "Source/Items/ItemData.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameMode.h"
#include <Source/GameMode/CustomGameModeBase.h>
#include <Kismet/GameplayStatics.h>
#include <Source/Utils/CheckUtils.h>

/// <summary>
/// Spawns an item in the world with the given parameters
/// </summary>
/// <param name="World"></param>
/// <param name="Transform"></param>
/// <param name="ItemClass"></param>
/// <param name="AddTTL"> If the item should be destroyed after the specified TTL in the game mode</param>
/// <returns></returns>
TObjectPtr<AItem> UItemFactory::SpawnItemInWorld(UWorld& World, const FTransform& Transform, TSubclassOf<AItem> ItemClass, const FGuid& ItemId, const int& Quantity, const bool& AddTTL)
{
	CHECK_NULLPTR(ItemClass);

	TObjectPtr<ACustomGameModeBase> pGameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(&World));
	CHECK_NULLPTR(pGameMode);

	TObjectPtr<UDataTable> pItemDataTable = pGameMode->ItemsDataTable;
	CHECK_NULLPTR(pItemDataTable);

	TObjectPtr<AItem> pItem = World.SpawnActorDeferred<AItem>(ItemClass, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding);
	CHECK_NULLPTR(pItem);

	FName rowName = FName(ItemId.ToString(EGuidFormats::DigitsWithHyphens));

	if (AddTTL)
	{
		// Get the item drop TTL settings from the GameMode
		float fItemTTL = pGameMode->DroppedItemTimeToLiveSeconds;
	
		pItem->SetDataWithTTL(rowName, Quantity, fItemTTL);
	}
	else
	{
		// Item valid forever
		pItem->SetData(rowName, Quantity);
	}

	return Cast<AItem>(UGameplayStatics::FinishSpawningActor(pItem, Transform));
}

/// <summary>
/// Spawns an item that is valid forever in the world
/// </summary>
/// <param name="World"></param>
/// <param name="Transform"></param>
/// <param name="ItemClass"></param>
/// <param name="Quantity"></param>
/// <returns></returns>
TObjectPtr<class AItem> UItemFactory::SpawnBPDefaultItemInWorld(UWorld& World, const FTransform& Transform, TSubclassOf<class AItem> ItemClass, const int& Quantity)
{
	CHECK_NULLPTR(ItemClass);

	TObjectPtr<ACustomGameModeBase> pGameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(&World));
	CHECK_NULLPTR(pGameMode);

	TObjectPtr<UDataTable> pItemDataTable = pGameMode->ItemsDataTable;
	CHECK_NULLPTR(pItemDataTable);

	TObjectPtr<AItem> pItem = World.SpawnActorDeferred<AItem>(ItemClass, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding);
	CHECK_NULLPTR(pItem);

	FName rowName = pItem->ItemDataRowName;

	// Item valid forever
	// Since it was spawned via BP default, we keep the original values
	pItem->SetData(rowName, Quantity);

	return Cast<AItem>(UGameplayStatics::FinishSpawningActor(pItem, Transform));
}
