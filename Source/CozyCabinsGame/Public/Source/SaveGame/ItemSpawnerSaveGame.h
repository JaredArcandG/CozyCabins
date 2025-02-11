// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include <Source/Items/ItemSpawner.h>
#include "ItemSpawnerSaveGame.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct COZYCABINSGAME_API FItemSpawnerSaveGameData
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FVector ActorLocation = FVector();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FRotator ActorRotation = FRotator();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FVector ActorScale = FVector();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FTransform ActorTransform = FTransform();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FItemSpawnSettings SpawnSettings = FItemSpawnSettings();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FDateTime PickupTime = FDateTime();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	bool bAwaitingRespawnAfterPlayerPickup = false;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FTimespan RespawnTimeInGameTime = FTimespan();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	bool bOverrideItemDataRespawnSettings = false;

};

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemSpawnerSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()
	
public:

	UItemSpawnerSaveGame();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TMap<FGuid, FItemSpawnerSaveGameData> DataMap;


public:

	virtual void OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave) override;

	virtual void OnLoadUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave) override;

	virtual void ClearOverwrite() override;
};
