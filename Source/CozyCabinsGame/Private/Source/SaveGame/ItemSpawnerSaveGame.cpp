// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/ItemSpawnerSaveGame.h"
#include <Source/Utils/CheckUtils.h>

UItemSpawnerSaveGame::UItemSpawnerSaveGame()
{
	ActorLocation = FVector();
	ActorRotation = FRotator();
	ActorScale = FVector();
	PickupTime = FDateTime::UtcNow();
	bAwaitingRespawnAfterPlayerPickup = false;
	RespawnTimeInGameTime = FTimespan::Zero();
	bOverrideItemDataRespawnSettings = false;
}

void UItemSpawnerSaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<AItemSpawner> pItemSpawner = Cast<AItemSpawner>(&ObjectToSave);
	CHECK(pItemSpawner);

	ActorLocation = pItemSpawner->GetActorLocation();
	ActorRotation = pItemSpawner->GetActorRotation();
	ActorScale = pItemSpawner->GetActorScale3D();
	SpawnSettings = pItemSpawner->SpawnSettings;
	PickupTime = pItemSpawner->PickupTime;
	bAwaitingRespawnAfterPlayerPickup = pItemSpawner->bAwaitingRespawnAfterPlayerPickup;
	RespawnTimeInGameTime = pItemSpawner->RespawnTimeInGameTime;
	bOverrideItemDataRespawnSettings = pItemSpawner->bOverrideItemDataRespawnSettings;
}

void UItemSpawnerSaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<AItemSpawner> pItemSpawner = Cast<AItemSpawner>(&ObjectToLoad);
	CHECK(pItemSpawner);

	pItemSpawner->SetActorLocation(ActorLocation);
	pItemSpawner->SetActorRotation(ActorRotation);
	pItemSpawner->SetActorScale3D(ActorScale);
	pItemSpawner->SpawnSettings = SpawnSettings;
	pItemSpawner->PickupTime = PickupTime;
	pItemSpawner->bAwaitingRespawnAfterPlayerPickup = bAwaitingRespawnAfterPlayerPickup;
	pItemSpawner->RespawnTimeInGameTime = RespawnTimeInGameTime;
	pItemSpawner->bOverrideItemDataRespawnSettings = bOverrideItemDataRespawnSettings;
}
