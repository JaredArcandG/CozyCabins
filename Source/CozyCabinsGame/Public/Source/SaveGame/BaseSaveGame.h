// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Source/SaveGame/ISaveGame.h"
#include "BaseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UBaseSaveGame : public USaveGame, public ICustomSaveGame
{
	GENERATED_BODY()
	
	// Information to save

	// 1. Player Inventory Component (Items) [DONE]

	// 2. Chest Inventory Components (Chest ID -> ItemValues) [DONE]

	// 3. Player Stats Component (Health, Hunger, Stamina) [DONE]

	// 4. ItemSpawners (Location, Last respawn, respawn state) [DONE]

	// 5. Crafting Data

	// 6. Player Location/Rotation [DONE]

	// 7. Game Time [DONE]

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;

};
