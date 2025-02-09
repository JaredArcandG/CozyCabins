// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/ItemSpawnerSaveGame.h"
#include <Source/Utils/CheckUtils.h>

UItemSpawnerSaveGame::UItemSpawnerSaveGame()
{
	ActorLocation = FVector();
	ActorRotation = FRotator();
	ActorScale = FVector();
	ActorTransform = FTransform();
	PickupTime = FDateTime::UtcNow();
	bAwaitingRespawnAfterPlayerPickup = false;
	RespawnTimeInGameTime = FTimespan::Zero();
	bOverrideItemDataRespawnSettings = false;
}

void UItemSpawnerSaveGame::OnSave(const UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<AItemSpawner> pItemSpawner = Cast<AItemSpawner>(&ObjectToSave);
	CHECK(pItemSpawner);

	ActorLocation = pItemSpawner->GetActorLocation();
	ActorRotation = pItemSpawner->GetActorRotation();
	ActorScale = pItemSpawner->GetActorScale3D();
	ActorTransform = pItemSpawner->GetActorTransform();
	SpawnSettings = pItemSpawner->SpawnSettings;
	PickupTime = pItemSpawner->PickupTime;
	bAwaitingRespawnAfterPlayerPickup = pItemSpawner->bAwaitingRespawnAfterPlayerPickup;
	RespawnTimeInGameTime = pItemSpawner->RespawnTimeInGameTime;
	bOverrideItemDataRespawnSettings = pItemSpawner->bOverrideItemDataRespawnSettings;
}

void UItemSpawnerSaveGame::OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<AItemSpawner> pItemSpawner = Cast<AItemSpawner>(&ObjectToLoad);
	CHECK(pItemSpawner);

	pItemSpawner->SetActorLocation(ActorLocation);
	pItemSpawner->SetActorRotation(ActorRotation);
	pItemSpawner->SetActorScale3D(ActorScale);
	pItemSpawner->SetActorTransform(ActorTransform);
	pItemSpawner->SpawnSettings = SpawnSettings;
	pItemSpawner->PickupTime = PickupTime;
	pItemSpawner->bAwaitingRespawnAfterPlayerPickup = bAwaitingRespawnAfterPlayerPickup;
	pItemSpawner->RespawnTimeInGameTime = RespawnTimeInGameTime;
	pItemSpawner->bOverrideItemDataRespawnSettings = bOverrideItemDataRespawnSettings;
}
