// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/GameTimeSaveGame.h"
#include <Source/Time/GameTimeManager.h>
#include <Source/Utils/CheckUtils.h>

UGameTimeSaveGame::UGameTimeSaveGame()
{
	CurrentGameTime = FDateTime::UtcNow();
}

void UGameTimeSaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<UGameTimeManager> pManager = Cast<UGameTimeManager>(&ObjectToSave);
	CHECK(pManager);

	CurrentGameTime = pManager->GetCurrentGameTime();
}

void UGameTimeSaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<UGameTimeManager> pManager = Cast<UGameTimeManager>(&ObjectToLoad);
	CHECK(pManager);

	pManager->SetCurrentGameTime(CurrentGameTime);
}

