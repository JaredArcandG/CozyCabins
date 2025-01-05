// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TimerHandle.h"
#include "GameTimeManager.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FCustomGameTime
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Year = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Month = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Day = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hour = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Minutes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seconds = 0;

};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYCABINSGAME_API UGameTimeManager : public UActorComponent
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameMinutePassed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameHourPassed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameDayPassed);

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one minute has passed in game time"))
	FOnGameMinutePassed OnGameMinutePassed;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one hour has passed in game time"))
	FOnGameHourPassed OnGameHourPassed;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one day has passed in game time"))
	FOnGameDayPassed OnGameDayPassed;

	UGameTimeManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", meta = (Text = "How many mintues in real time represent one hour in game time"))
	float RealTimeMinutesPerGameHour;

protected:

	FTimerHandle GameTimerHandle;

	FCustomGameTime CurrentGameTime;

public:

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Increment the game time by the specified parameters"))
	void IncrementGameTime(const int& InAddYears, const int& InAddMonths, const int& InAddDays, const int& InAddHours, const int& InAddMinutes, const int& AddSeconds);

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the current game time"))
	FCustomGameTime GetCurrentGameTime();

protected:

	void BroadCastGameTimeMinutePassed();

	void BroadCastGameTimeHourPassed();

	void BroadCastGameTimeDayPassed();

	void IncrementGameTimeYears(const int& InAddYears);

	void IncrementGameTimeMonths(const int& InAddMonths);

	void IncrementGameTimeDays(const int& InAddDays);

	void IncrementGameTimeHours(const int& InAddHours);

	void IncrementGameTimeMinutes(const int& InAddMinutes);

	void IncrementGameTimeSeconds(const int& InAddSeconds);

};
