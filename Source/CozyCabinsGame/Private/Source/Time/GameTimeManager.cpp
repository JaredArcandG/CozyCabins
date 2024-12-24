// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Time/GameTimeManager.h"

UGameTimeManager::UGameTimeManager()
{
	RealTimeMinutesPerGameHour = 2.5f;

}

void UGameTimeManager::BeginPlay()
{
	Super::BeginPlay();

	float fRealtimeSecs = 60 * RealTimeMinutesPerGameHour;

	// Broadcast event for game time passed every fRealtime seconds
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, FTimerDelegate::CreateUObject(this, &UGameTimeManager::BroadCastGameTimePassed), fRealtimeSecs, true);
}

void UGameTimeManager::BroadCastGameTimePassed()
{
	OnGameMinutePassed.Broadcast();
}
