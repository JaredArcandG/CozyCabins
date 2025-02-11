// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "PlayerInventorySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UPlayerInventorySaveGame : public UBaseSaveGame
{
	GENERATED_BODY()

public:

	UPlayerInventorySaveGame();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int MaxInventorySize;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int MaxItemStackSize;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	bool bCanUseInventory;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<int> ItemQuantityArr;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> ItemIdArr;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int CurrentSize;

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;
	
};
