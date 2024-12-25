// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Time/GameTimeManager.h"

/// <summary>
/// ctor
/// </summary>
UGameTimeManager::UGameTimeManager()
{
	RealTimeMinutesPerGameHour = 2.5f;

}

void UGameTimeManager::BeginPlay()
{
	Super::BeginPlay();

	float fRealtimeSecs = RealTimeMinutesPerGameHour; // multiply by 60 to get real time seconds, divide by 60 to get real time seconds per game minute, they cancel out

	// Broadcast event for game time passed every fRealtime seconds
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, FTimerDelegate::CreateUObject(this, &UGameTimeManager::BroadCastGameTimePassed), fRealtimeSecs, true);
}

/// <summary>
/// Broadcasts when one game minute has passed
/// Can be hooked via the OnGameMinutePassed event to allow various time related functionality
/// </summary>
void UGameTimeManager::BroadCastGameTimePassed()
{
	OnGameMinutePassed.Broadcast();
}
