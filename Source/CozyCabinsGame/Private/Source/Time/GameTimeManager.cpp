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

	// Number of real time seconds that represent one game minute
	float fRealtimeSecsGameMinute = RealTimeMinutesPerGameHour;

	// Broadcast event for game time passed every fRealtime seconds
	GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &UGameTimeManager::BroadCastGameTimeMinutePassed, fRealtimeSecsGameMinute, true);
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
	// Disable the timer to avoid timer/increment collision
	GetWorld()->GetTimerManager().PauseTimer(GameTimerHandle);
	
	// Since timespan can only increase by a day max, convert years and months to days
	int totalDays = InAddDays + (365 * InAddYears) + (30 * InAddMonths);
	FTimespan incrementTS(totalDays, InAddHours, InAddMinutes, InAddSeconds);

	CurrentGameTime += incrementTS;

	// Broadcasts that X amount of time has passed
	OnGameTimePassed.Broadcast(incrementTS, CurrentGameTime);

	// Re-enable the timer
	GetWorld()->GetTimerManager().UnPauseTimer(GameTimerHandle);
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
/// Increments the game time by one minute
/// </summary>
void UGameTimeManager::BroadCastGameTimeMinutePassed()
{
	// Increment game time by one minute
	IncrementGameTime(0, 0, 0, 0, 1, 0);
}
