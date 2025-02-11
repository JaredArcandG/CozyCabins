// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include "InventorySaveGame.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct COZYCABINSGAME_API FInventorySaveGameData
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int MaxInventorySize = 0;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int MaxItemStackSize = 0;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	bool bCanUseInventory = 0;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<int> ItemQuantityArr;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<FGuid> ItemIdArr;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	int CurrentSize = 0;
};

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UInventorySaveGame : public UBaseSaveGame
{
	GENERATED_BODY()

public:

	UInventorySaveGame();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FInventorySaveGameData> DataMap;


public:

	virtual void OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave) override;

	virtual void OnLoadUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToLoad) override;

	virtual void ClearOverwrite() override;
	
};
