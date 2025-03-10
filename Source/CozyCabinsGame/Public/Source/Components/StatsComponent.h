// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

#define MIN_TOTAL_STAT_VALUE 0
#define MAX_TOTAL_STAT_VALUE 100

UENUM(BlueprintType, Blueprintable)
enum class EActorStatType : uint8
{
	Health,
	Stamina,
	Hunger
};

USTRUCT(BlueprintType, Blueprintable)
struct COZYCABINSGAME_API FActorStats
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentHunger = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TotalHunger = 100;

};

/// <summary>
/// Component that controls the actor's statistics like stamina and health
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COZYCABINSGAME_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, EActorStatType, StatType);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatChanged OnStatChanged;

	// Sets default values for this component's properties
	UStatsComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FActorStats ActorStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HungerDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Text="How much to decay health based on hunger decay gametime mins if hunger is zero"))
	int HungerHealthDecayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HungerDecayGametimeMins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UPlayerDeathWidget> DeathScreenWidgetClass;

	UPROPERTY()
	TObjectPtr<class UPlayerDeathWidget> DeathScreenWidget;

	FDateTime TimeSinceLastHungerDecay;

	UPROPERTY()
	TObjectPtr<class UGameTimeManager> GameTimeManager;

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the current health of the actor"))
	int GetCurrentHealth();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the total health of the actor"))
	int GetTotalHealth();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the current stamina of the actor"))
	int GetCurrentStamina();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the total stamina of the actor"))
	int GetTotalStamina();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the current hunger of the actor"))
	int GetCurrentHunger();

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the total hunger of the actor"))
	int GetTotalHunger();

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the current health of the actor"))
	void SetCurrentHealth(const int& NewHealth);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the total health of the actor"))
	void SetTotalHealth(const int& NewHealth);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the current stamina of the actor"))
	void SetCurrentStamina(const int& NewStamina);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the total stamina of the actor"))
	void SetTotalStamina(const int& NewStamina);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the current hunger of the actor"))
	void SetCurrentHunger(const int& NewHunger);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the total hunger of the actor"))
	void SetTotalHunger(const int& NewHunger);

	float GetHealthRatio();

	float GetStaminaRatio();

	float GetHungerRatio();

	FDateTime GetTimeSinceLastHungerDecay();

	void SetTimeSinceLastHungerDecay(const FDateTime& DateTime);

	int GetHungerDecayRate();

	void SetHungerDecayRate(const int& DecayRate);

	int GetHungerDecayGameTimeMins();

	void SetHungerDecayGameTimeMins(const int& DecayTime);

protected:

	UFUNCTION()
	void DecayHunger(FTimespan Timespan, FDateTime CurrentDateTime);

	UFUNCTION()
	void OnPlayerDeathEnd();

	void OnPlayerDeathStart();
		
};
