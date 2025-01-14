// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Time/GameTimeManager.h"

/// <summary>
/// ctor
/// </summary>
UGameTimeManager::UGameTimeManager()
{
	RealTimeMinutesPerGameHour = 2.5f;

	CurrentGameTime = FDateTime(1, 1, 1);

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
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandleMinute, this, &UGameTimeManager::BroadCastGameTimeMinutePassed, fRealtimeSecsGameMinute, true);
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandleHour, this, &UGameTimeManager::BroadCastGameTimeHourPassed, fRealtimeSecsGameHour, true);
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandleDay, this, &UGameTimeManager::BroadCastGameTimeDayPassed, fRealtimeSecsGameDay, true);
}

/// <summary>
/// Increments the game time by the specified parameters
/// </summary>
/// <param name="InAddYears"></param>
/// <param name="InAddMonths"></param>
/// <param name="InAddDays"></param>
/// <param name="InAddHours"></param>
/// <param name="InAddMinutes"></param>
/// <param name="AddSeconds"></param>
void UGameTimeManager::IncrementGameTime(const int& InAddYears, const int& InAddMonths, const int& InAddDays, const int& InAddHours, const int& InAddMinutes, const int& InAddSeconds)
{
	// Since timespan can only increase by a day max, convert years and months todays
	int totalDays = InAddDays + (365 * InAddYears) + (30 * InAddMonths);
	FTimespan incrementTS(totalDays, InAddHours, InAddMinutes, InAddSeconds);

	CurrentGameTime += incrementTS;
}

/// <summary>
/// Gets the current game time for the game
/// </summary>
/// <returns></returns>
FDateTime UGameTimeManager::GetCurrentGameTime()
{
	return CurrentGameTime;
}

/// <summary>
/// Gets the time difference between the current game time and the input time
/// </summary>
/// <param name="InTime"></param>
/// <returns></returns>
FTimespan UGameTimeManager::GetTimeDifferenceFromCurrentTime(const FDateTime& InTime)
{
	return CurrentGameTime - InTime;
}

/// <summary>
/// Broadcasts when one game minute has passed
/// Can be hooked via the OnGameMinutePassed event to allow various time related functionality
/// </summary>
void UGameTimeManager::BroadCastGameTimeMinutePassed()
{
	OnGameMinutePassed.Broadcast();

	// We only want to increment on the lowest broadcast
	// If we add this in other broadcasts, we are double counting
	FTimespan minute(0, 1, 0);
	CurrentGameTime += minute;
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
