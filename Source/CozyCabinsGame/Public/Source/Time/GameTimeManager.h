// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TimerHandle.h"
#include "GameTimeManager.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FCustomTimespan
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Years = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Months = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Days = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hours = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Minutes = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seconds = 0;

	/// <summary>
	/// Converts custom timespan to actual FTimespan
	/// </summary>
	/// <returns></returns>
	FTimespan ConvertToTimespan()
	{
		int daysInYears = Years * 365;
		int daysInMonths = Months * 30;

		return FTimespan(daysInYears + daysInMonths + Days, Hours, Minutes, Seconds);
	}

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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameTimePassed, FTimespan, TimePassed, FDateTime, CurGameTime);

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one minute has passed in game time"))
	FOnGameMinutePassed OnGameMinutePassed;

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one day has passed in game time"))
	FOnGameTimePassed OnGameTimePassed;

	UGameTimeManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", meta = (Text = "How many mintues in real time represent one hour in game time"))
	float RealTimeMinutesPerGameHour;

protected:

	FTimerHandle GameTimerHandle;

	FDateTime CurrentGameTime;

public:

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Increment the game time by the specified parameters"))
	void IncrementGameTime(const int& InAddYears, const int& InAddMonths, const int& InAddDays, const int& InAddHours, const int& InAddMinutes, const int& AddSeconds);

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the current game time"))
	FDateTime GetCurrentGameTime();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the time difference from input time"))
	FTimespan GetTimeDifferenceFromCurrentTime(const FDateTime& InTime);

protected:

	UFUNCTION()
	void BroadCastGameTimeMinutePassed();

};
