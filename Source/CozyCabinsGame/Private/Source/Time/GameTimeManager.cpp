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

	// Number of real time seconds that represent one game hour
	float fRealtimeSecsGameHour = 60 * RealTimeMinutesPerGameHour;

	// Number of real time seconds that represent one game minute
	float fRealtimeSecsGameMinute = fRealtimeSecsGameHour / 60;

	// Number of real time seconds that represent one game day
	float fRealtimeSecsGameDay = fRealtimeSecsGameHour * 60;

	// Broadcast event for game time passed every fRealtime seconds
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, FTimerDelegate::CreateUObject(this, &UGameTimeManager::BroadCastGameTimeMinutePassed), fRealtimeSecsGameMinute, true);
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, FTimerDelegate::CreateUObject(this, &UGameTimeManager::BroadCastGameTimeHourPassed), fRealtimeSecsGameHour, true);
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, FTimerDelegate::CreateUObject(this, &UGameTimeManager::BroadCastGameTimeDayPassed), fRealtimeSecsGameDay, true);
}

/// <summary>
/// Broadcasts when one game minute has passed
/// Can be hooked via the OnGameMinutePassed event to allow various time related functionality
/// </summary>
void UGameTimeManager::BroadCastGameTimeMinutePassed()
{
	OnGameMinutePassed.Broadcast();
}

/// <summary>
/// Broadcasts when one game hour has passed
/// Can be hooked via OnGameHourPassed event to allow various time related functionality
/// </summary>
void UGameTimeManager::BroadCastGameTimeHourPassed()
{
	OnGameHourPassed.Broadcast();
}

/// <summary>
/// Broadcasts when one game day has passed
/// Can be hooked via OnGameDayPassed event to allow various time related functionality
/// </summary>
void UGameTimeManager::BroadCastGameTimeDayPassed()
{
	OnGameDayPassed.Broadcast();
}
