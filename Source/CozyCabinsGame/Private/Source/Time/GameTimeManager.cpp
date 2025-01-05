// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Time/GameTimeManager.h"

/// <summary>
/// ctor
/// </summary>
UGameTimeManager::UGameTimeManager()
{
	RealTimeMinutesPerGameHour = 2.5f;

	CurrentGameTime = FCustomGameTime();

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
	IncrementGameTimeYears(InAddYears);
	IncrementGameTimeMonths(InAddMonths);
	IncrementGameTimeDays(InAddDays);
	IncrementGameTimeHours(InAddHours);
	IncrementGameTimeMinutes(InAddMinutes);
	IncrementGameTimeSeconds(InAddSeconds);
}

FCustomGameTime UGameTimeManager::GetCurrentGameTime()
{
	return CurrentGameTime;
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
	IncrementGameTimeMinutes(1);
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

void UGameTimeManager::IncrementGameTimeYears(const int& InAddYears)
{
	CurrentGameTime.Year += InAddYears;
}

void UGameTimeManager::IncrementGameTimeMonths(const int& InAddMonths)
{
	int yearsToAdd = InAddMonths / 12;
	int monthsToAdd = InAddMonths % 12;

	if (yearsToAdd > 0)
	{
		IncrementGameTimeYears(yearsToAdd);
	}

	CurrentGameTime.Month += monthsToAdd;

	if (CurrentGameTime.Month >= 12)
	{
		CurrentGameTime.Month = CurrentGameTime.Month % 12;
		IncrementGameTimeYears(1);
	}
}

void UGameTimeManager::IncrementGameTimeDays(const int& InAddDays)
{
	int monthsToAdd = InAddDays / 30;

	if (monthsToAdd > 0)
	{
		IncrementGameTimeMonths(monthsToAdd);
	}

	int daysToAdd = InAddDays % 30;
	CurrentGameTime.Day += daysToAdd;

	// Check post overflow
	if (CurrentGameTime.Day >= 30)
	{
		CurrentGameTime.Day = CurrentGameTime.Day % 30;
		IncrementGameTimeMonths(1);
	}
}

void UGameTimeManager::IncrementGameTimeHours(const int& InAddHours)
{
	int daysToAdd = InAddHours / 24;

	if (daysToAdd > 0)
	{
		IncrementGameTimeDays(daysToAdd);
	}

	int hoursToAdd = InAddHours % 24;
	CurrentGameTime.Hour += hoursToAdd;

	// Check post overflow
	if (CurrentGameTime.Hour >= 24)
	{
		CurrentGameTime.Hour = CurrentGameTime.Hour % 24;
		IncrementGameTimeDays(1);
	}
}

void UGameTimeManager::IncrementGameTimeMinutes(const int& InAddMinutes)
{
	int hoursToAdd = InAddMinutes / 60;

	if (hoursToAdd > 0)
	{
		IncrementGameTimeHours(hoursToAdd);
	}

	int minsToAdd = InAddMinutes % 60;
	CurrentGameTime.Minutes += minsToAdd;

	// Check post overflow
	if (CurrentGameTime.Minutes >= 60)
	{
		CurrentGameTime.Minutes = CurrentGameTime.Minutes % 60;
		IncrementGameTimeHours(1);
	}
}

void UGameTimeManager::IncrementGameTimeSeconds(const int& InAddSeconds)
{
	int minsToAdd = InAddSeconds / 60;
	
	if (minsToAdd > 0)
	{
		IncrementGameTimeMinutes(minsToAdd);
	}

	int secsToAdd = InAddSeconds % 60;
	CurrentGameTime.Seconds += secsToAdd;

	// Check post overflow
	if (CurrentGameTime.Seconds >= 60)
	{
		CurrentGameTime.Seconds = CurrentGameTime.Seconds % 60;
		IncrementGameTimeMinutes(1);
	}
}
