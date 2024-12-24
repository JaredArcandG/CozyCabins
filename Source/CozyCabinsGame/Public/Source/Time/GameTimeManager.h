// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TimerHandle.h"
#include "GameTimeManager.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COZYCABINSGAME_API UGameTimeManager : public UActorComponent
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameMinutePassed); 

	UPROPERTY(BlueprintAssignable, Category = "Events", meta = (Text = "Event that is broadcasted when one minute has passed in game time"))
	FOnGameMinutePassed OnGameMinutePassed;

	UGameTimeManager();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings", meta = (Text = "How many mintues in real time represent one hour in game time"))
	float RealTimeMinutesPerGameHour;

protected:

	FTimerHandle GameTimerHandle;

public:

	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	void BroadCastGameTimePassed();
};
