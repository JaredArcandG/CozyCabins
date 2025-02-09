// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/BaseSaveGame.h"
#include <Source/Items/ItemSpawner.h>
#include "ItemSpawnerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemSpawnerSaveGame : public UBaseSaveGame
{
	GENERATED_BODY()
	
public:

	UItemSpawnerSaveGame();

	UPROPERTY()
	FVector ActorLocation;

	UPROPERTY()
	FRotator ActorRotation;

	UPROPERTY()
	FVector ActorScale;

	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	FItemSpawnSettings SpawnSettings;

	UPROPERTY()
	FDateTime PickupTime;

	UPROPERTY()
	bool bAwaitingRespawnAfterPlayerPickup;

	UPROPERTY()
	FTimespan RespawnTimeInGameTime;

	UPROPERTY()
	bool bOverrideItemDataRespawnSettings;

public:

	virtual void OnSave(const UObject& WorldContextObject, UObject& ObjectToSave) override;

	virtual void OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad) override;
};
