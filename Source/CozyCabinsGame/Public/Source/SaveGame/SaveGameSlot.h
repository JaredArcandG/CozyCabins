// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Source/SaveGame/ISaveGame.h"
#include "SaveGameSlot.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API USaveGameSlot : public UObject
{
	GENERATED_BODY()

protected:

	FString BaseSaveName;

	int SaveIndex;

	// To add a new save component, add the savegame to the class here
	// The struct is populated by the OnLoad and OnSave functions
	// Use the OnSave function to populate the data
	// Use the OnLoad function for any actions after loading

	// The player core info (Location, Rotation, etc.)
	UPROPERTY()
	TObjectPtr<class UPlayerCoreSaveGame> PlayerCoreSaveGame;
	FString PlayerCoreSaveGameExtString;

	// The player's inventory
	UPROPERTY()
	TObjectPtr<class UPlayerInventorySaveGame> PlayerInventorySaveGame;
	FString PlayerInventorySaveGameExtString;

	// All Inventories in the game - mapped by Unique ID of the actor
	UPROPERTY()
	TObjectPtr<class UInventorySaveGame> InventorySaveGame;
	FString InventorySaveGameExtString;

	// The player's stats
	UPROPERTY()
	TObjectPtr<class UPlayerStatsSaveGame> PlayerStatsSaveGame;
	FString PlayerStatsSaveGameExtString;

	// Items
	UPROPERTY()
	TObjectPtr<class UItemSpawnerSaveGame> ItemSpawnerSaveGame;
	FString ItemSpawnerSaveGamesExtString;

	// Game Time
	UPROPERTY()
	TObjectPtr<class UGameTimeSaveGame> GameTimeSaveGame;
	FString GameTimeSaveGameExtString;

	// Crafting
	UPROPERTY()
	TObjectPtr<class UCraftingSaveGame> CraftingSaveGame;
	FString CraftingSaveGameExtString;

	UPROPERTY()
	TSubclassOf<class UCraftingSaveGame> CraftingSaveGameClass;

	UPROPERTY()
	TArray<TObjectPtr<class UDataTable>> CraftingDataTables;

public:

	void Setup(const FString& InBaseSaveName, const int& InSaveIndex, const TSubclassOf<class UCraftingSaveGame>& InCraftingSaveGameClass, const TArray<TObjectPtr<class UDataTable>>& InCraftingDataTables);

	void ProcessSaveSlot(const UObject& WorldContextObject);

	void ProcessLoadSlot(const UObject& WorldContextObject);

protected:

	template <typename T>
	T* PreProcessSaveGame(const FString& InSlotExt, const int& InSlotIndex);

	template <typename T>
	T* PreProcessSaveGame(const TSubclassOf<T>& SaveGameClass, const FString& InSlotExt, const int32 InSlotIndex);
};
