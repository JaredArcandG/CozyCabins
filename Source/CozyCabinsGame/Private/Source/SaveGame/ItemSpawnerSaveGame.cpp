// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/ItemSpawnerSaveGame.h"
#include <Source/Utils/CheckUtils.h>

UItemSpawnerSaveGame::UItemSpawnerSaveGame()
{

}

void UItemSpawnerSaveGame::OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave)
{
	TObjectPtr<AItemSpawner> pItemSpawner = Cast<AItemSpawner>(&ObjectToSave);
	CHECK(pItemSpawner);

	FItemSpawnerSaveGameData SaveData;

	SaveData.ActorLocation = pItemSpawner->GetActorLocation();
	SaveData.ActorRotation = pItemSpawner->GetActorRotation();
	SaveData.ActorScale = pItemSpawner->GetActorScale3D();
	SaveData.ActorTransform = pItemSpawner->GetActorTransform();
	SaveData.SpawnSettings = pItemSpawner->SpawnSettings;
	SaveData.PickupTime = pItemSpawner->PickupTime;
	SaveData.bAwaitingRespawnAfterPlayerPickup = pItemSpawner->bAwaitingRespawnAfterPlayerPickup;
	SaveData.RespawnTimeInGameTime = pItemSpawner->RespawnTimeInGameTime;
	SaveData.bOverrideItemDataRespawnSettings = pItemSpawner->bOverrideItemDataRespawnSettings;

	TTuple<FGuid, FItemSpawnerSaveGameData> kvp = TTuple<FGuid, FItemSpawnerSaveGameData>();
	kvp.Key = ObjId;
	kvp.Value = SaveData;

	DataMap.Add(kvp);
}

void UItemSpawnerSaveGame::ClearOverwrite()
{
	DataMap.Empty();
}
