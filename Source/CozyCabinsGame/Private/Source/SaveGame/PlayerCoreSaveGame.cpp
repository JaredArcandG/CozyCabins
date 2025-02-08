// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/PlayerCoreSaveGame.h"
#include <Source/Player/PlayerCharacter.h>
#include <Source/Utils/CheckUtils.h>

UPlayerCoreSaveGame::UPlayerCoreSaveGame()
{
	PlayerLocation = FVector::Zero();
	PlayerRotation = FRotator();
}

void UPlayerCoreSaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(&ObjectToSave);
	CHECK(pPlayer);

	PlayerLocation = pPlayer->GetActorLocation();
	PlayerRotation = pPlayer->GetActorRotation();
}

void UPlayerCoreSaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(&ObjectToLoad);
	CHECK(pPlayer);

	pPlayer->SetActorLocation(PlayerLocation);
	pPlayer->SetActorRotation(PlayerRotation);
}
