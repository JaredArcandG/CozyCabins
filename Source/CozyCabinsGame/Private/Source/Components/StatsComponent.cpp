// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Components/StatsComponent.h"
#include <Source/Time/GameTimeManager.h>
#include <Kismet/GameplayStatics.h>
#include <Source/Utils/CheckUtils.h>
#include "GameFramework/GameModeBase.h"
#include <Source/Player/PlayerCharacter.h>
#include <Source/Player/Spawn/SpawnPoint.h>
#include <Source/Utils/CommonUtils.h>
#include "Source/UI/Death/PlayerDeathWidget.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	HungerDecayRate = 1;
	HungerHealthDecayRate = 1;
	HungerDecayGametimeMins = 1;

	// ...
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AGameModeBase> pGameMode = UGameplayStatics::GetGameMode(GetWorld());
	CHECK(pGameMode);

	GameTimeManager = pGameMode->FindComponentByClass<UGameTimeManager>();
	CHECK(GameTimeManager);

	TimeSinceLastHungerDecay = GameTimeManager->GetCurrentGameTime();

	GameTimeManager->OnGameTimePassed.AddUniqueDynamic(this, &UStatsComponent::DecayHunger);

	CHECK(DeathScreenWidgetClass);

	DeathScreenWidget = CreateWidget<UPlayerDeathWidget>(GetWorld()->GetFirstPlayerController(), DeathScreenWidgetClass);
	CHECK(DeathScreenWidget);

	DeathScreenWidget->AddToViewport();

	DeathScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	
}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/// <summary>
/// Gets the current health of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetCurrentHealth()
{
	return ActorStats.CurrentHealth;
}

/// <summary>
/// Gets the total/base health of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetTotalHealth()
{
	return ActorStats.TotalHealth;
}

/// <summary>
/// Gets the current stamina of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetCurrentStamina()
{
	return ActorStats.CurrentStamina;
}

/// <summary>
/// Gets the total/base stamina of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetTotalStamina()
{
	return ActorStats.TotalStamina;
}

/// <summary>
/// Gets the current hunger of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetCurrentHunger()
{
	return ActorStats.CurrentHunger;
}

/// <summary>
/// Gets the total hunger of the actor
/// </summary>
/// <returns></returns>
int UStatsComponent::GetTotalHunger()
{
	return ActorStats.TotalHunger;
}

/// <summary>
/// Sets the current health of the actor
/// </summary>
/// <param name="NewHealth"></param>
void UStatsComponent::SetCurrentHealth(const int& NewHealth)
{
	ActorStats.CurrentHealth = FMath::Clamp(NewHealth, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Health);

	if (ActorStats.CurrentHealth == MIN_TOTAL_STAT_VALUE)
	{
		OnPlayerDeathStart();
	}
}

/// <summary>
/// Sets the base health of the actor
/// </summary>
/// <param name="NewHealth"></param>
void UStatsComponent::SetTotalHealth(const int& NewHealth)
{
	ActorStats.TotalHealth = FMath::Clamp(NewHealth, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	// Ensure current health is in the limit
	ActorStats.CurrentHealth = FMath::Clamp(ActorStats.CurrentHealth, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Health);
}

/// <summary>
/// Sets the current stamina of the actor
/// </summary>
/// <param name="NewStamina"></param>
void UStatsComponent::SetCurrentStamina(const int& NewStamina)
{
	ActorStats.CurrentStamina = FMath::Clamp(NewStamina, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Stamina);
}

/// <summary>
/// Sets the total stamina of the actor
/// </summary>
/// <param name="NewStamina"></param>
void UStatsComponent::SetTotalStamina(const int& NewStamina)
{
	ActorStats.TotalStamina = FMath::Clamp(NewStamina, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	// Ensure current stamina is in the limit 
	ActorStats.CurrentStamina = FMath::Clamp(ActorStats.CurrentStamina, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Stamina);
}

/// <summary>
/// Sets the current hunger of the actor
/// </summary>
/// <param name="NewHunger"></param>
void UStatsComponent::SetCurrentHunger(const int& NewHunger)
{
	ActorStats.CurrentHunger = FMath::Clamp(NewHunger, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Hunger);
}

/// <summary>
/// Sets the total/base hunger of the actor
/// </summary>
/// <param name="NewHunger"></param>
void UStatsComponent::SetTotalHunger(const int& NewHunger)
{
	ActorStats.TotalHunger = FMath::Clamp(NewHunger, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	// Ensure current hunger is in the limit 
	ActorStats.CurrentHunger = FMath::Clamp(ActorStats.CurrentHunger, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	OnStatChanged.Broadcast(EActorStatType::Hunger);
}

/// <summary>
/// Gets the health percentage
/// Useful for UI display
/// </summary>
/// <returns></returns>
float UStatsComponent::GetHealthRatio()
{
	if (FMath::IsNearlyZero(static_cast<float>(ActorStats.TotalHealth)))
	{
		return 0.f;
	}

	return static_cast<float>(ActorStats.CurrentHealth) / ActorStats.TotalHealth;
}

/// <summary>
/// Gets the stamina percentage
/// Useful for UI display
/// </summary>
/// <returns></returns>
float UStatsComponent::GetStaminaRatio()
{
	if (FMath::IsNearlyZero(static_cast<float>(ActorStats.TotalStamina)))
	{
		return 0.f;
	}

	return static_cast<float>(ActorStats.CurrentStamina) / ActorStats.TotalStamina;
}

/// <summary>
/// Gets the hunger percentage
/// Useful for UI Display
/// </summary>
/// <returns></returns>
float UStatsComponent::GetHungerRatio()
{
	if (FMath::IsNearlyZero(static_cast<float>(ActorStats.TotalHunger)))
	{
		return 0.f;
	}

	return static_cast<float>(ActorStats.CurrentHunger) / ActorStats.TotalHunger;
}

/// <summary>
/// Gets the last time the hunger decay was triggered
/// </summary>
/// <returns></returns>
FDateTime UStatsComponent::GetTimeSinceLastHungerDecay()
{
	return TimeSinceLastHungerDecay;
}

/// <summary>
/// Sets the last time the hunger decay was triggered
/// </summary>
/// <param name="DateTime"></param>
void UStatsComponent::SetTimeSinceLastHungerDecay(const FDateTime& DateTime)
{
	TimeSinceLastHungerDecay = DateTime;
}

/// <summary>
/// Gets the hunger decay rate
/// </summary>
/// <returns></returns>
int UStatsComponent::GetHungerDecayRate()
{
	return HungerDecayRate;
}

/// <summary>
/// Sets the hunger decay rate
/// </summary>
/// <param name="DecayRate"></param>
void UStatsComponent::SetHungerDecayRate(const int& DecayRate)
{
	HungerDecayRate = DecayRate;
}

/// <summary>
/// Gets the hunger decay gametime in mins
/// </summary>
/// <returns></returns>
int UStatsComponent::GetHungerDecayGameTimeMins()
{
	return HungerDecayGametimeMins;
}

/// <summary>
/// Sets the hunger decay game time (in minutes)
/// </summary>
/// <param name="DecayTime"></param>
void UStatsComponent::SetHungerDecayGameTimeMins(const int& DecayTime)
{
	HungerDecayGametimeMins = DecayTime;
}

/// <summary>
/// Decays hunger based on the specified decay class params
/// </summary>
void UStatsComponent::DecayHunger(FTimespan Timespan, FDateTime CurrentDateTime)
{
	CHECK(GameTimeManager);

	FTimespan tsHungerDecayGametimeMins = FTimespan(0, HungerDecayGametimeMins, 0);

	if (Timespan >= tsHungerDecayGametimeMins)
	{
		SetCurrentHunger(ActorStats.CurrentHunger - HungerDecayRate);
		TimeSinceLastHungerDecay = CurrentDateTime;

		if (ActorStats.CurrentHunger == MIN_TOTAL_STAT_VALUE)
		{
			// Take Damage
			SetCurrentHealth(GetCurrentHealth() - HungerHealthDecayRate);
		}
	}

}

void UStatsComponent::OnPlayerDeathStart()
{
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(GetOwner());
	CHECK(pPlayer);
	
	// Play animation
	CHECK(DeathScreenWidget);
	DeathScreenWidget->SetVisibility(ESlateVisibility::Visible);

	// On animation end, start the second part of death sequence
	if (DeathScreenWidget->OnDeath)
	{
		FWidgetAnimationDynamicEvent OnEndFadeOutEvent;
		OnEndFadeOutEvent.BindUFunction(this, FName("OnPlayerDeathEnd"));
		DeathScreenWidget->BindToAnimationFinished(DeathScreenWidget->OnDeath, OnEndFadeOutEvent);
		DeathScreenWidget->PlayAnimation(DeathScreenWidget->OnDeath);

		// Restrict movement
		pPlayer->bBlockInput = true;
	}

}

void UStatsComponent::OnPlayerDeathEnd()
{
	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(GetOwner());
	CHECK(pPlayer);

	// Respawn at respawn point
	auto pWorld = GetWorld();
	CHECK(pWorld);

	TArray<TObjectPtr<ASpawnPoint>> arrSpawnPoints = UCommonUtils::GetAllActorsInWorld<ASpawnPoint>(*pWorld);

	if (arrSpawnPoints.Num() > 0)
	{
		// Set health, stamina, hunger back to full
		SetCurrentHealth(GetTotalHealth());
		SetCurrentStamina(GetTotalStamina());
		SetCurrentHunger(GetTotalHunger());

		auto pSpawnPoint = arrSpawnPoints[0];
		CHECK(pSpawnPoint);

		pPlayer->SetActorLocation(pSpawnPoint->GetActorLocation());

		CHECK(DeathScreenWidget);
		DeathScreenWidget->SetVisibility(ESlateVisibility::Hidden);
		DeathScreenWidget->ResetWidget();

		// Restore movement
		pPlayer->bBlockInput = false;
	}
}


