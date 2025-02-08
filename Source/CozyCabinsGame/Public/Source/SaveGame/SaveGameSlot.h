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
class COZYCABINSGAME_API USaveGameSlot : public USaveGame
{
	GENERATED_BODY()

protected:

	// To add a new save component, add the savegame to the class here
	// Use the OnSave function to populate the data
	// Use the OnLoad function for any actions after loading

	// The player core info (Location, Rotation, etc.)
	UPROPERTY()
	TObjectPtr<class UPlayerCoreSaveGame> PlayerCoreSaveGame;

	// The player's inventory
	UPROPERTY()
	TObjectPtr<class UPlayerInventorySaveGame> PlayerInventorySaveGame;

	// All Inventories in the game - mapped by Unique ID of the actor
	UPROPERTY()
	TMap<FGuid,TObjectPtr<class UInventorySaveGame>> InventorySaveGames;

	// The player's stats
	UPROPERTY()
	TObjectPtr<class UPlayerStatsSaveGame> PlayerStatsSaveGame;

	// Items
	UPROPERTY()
	TMap<FGuid, TObjectPtr<class UItemSpawnerSaveGame>> ItemSpawnerSaveGames;

	// Game Time
	UPROPERTY()
	TObjectPtr<class UGameTimeSaveGame> GameTimeSaveGame;
	

public:

	void Setup();

	void SaveSlot(UObject& WorldContextObject, const FString& SaveSlot);

	void LoadSlot(UObject& WorldContextObject, const FString& SaveSlot);
	
};
