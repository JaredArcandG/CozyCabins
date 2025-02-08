// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "InventorySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UInventorySaveGame : public UBaseSaveGame
{
	GENERATED_BODY()

public:

	UInventorySaveGame();

	UPROPERTY()
	int MaxInventorySize;

	UPROPERTY()
	int MaxItemStackSize;

	UPROPERTY()
	bool bCanUseInventory;

	UPROPERTY()
	TArray<int> ItemQuantityArr;

	UPROPERTY()
	TArray<FGuid> ItemIdArr;

	UPROPERTY()
	int CurrentSize;

public:

	virtual void OnSave(UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad) override;
	
};
