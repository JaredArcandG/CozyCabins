// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Components/StatsComponent.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UStatsComponent::GetCurrentHealth()
{
	return ActorStats.CurrentHealth;
}

int UStatsComponent::GetTotalHealth()
{
	return ActorStats.TotalHealth;
}

int UStatsComponent::GetCurrentStamina()
{
	return ActorStats.CurrentStamina;
}

int UStatsComponent::GetTotalStamina()
{
	return ActorStats.TotalStamina;
}

void UStatsComponent::SetCurrentHealth(const int& NewHealth)
{
	ActorStats.CurrentHealth = FMath::Clamp(NewHealth, 0, ActorStats.TotalHealth);

	OnStatChanged.Broadcast(EActorStatType::Health);
}

void UStatsComponent::SetTotalHealth(const int& NewHealth)
{
	ActorStats.TotalHealth = FMath::Clamp(NewHealth, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	// Ensure current health is in the limit
	ActorStats.CurrentHealth = FMath::Clamp(ActorStats.CurrentHealth, 0, ActorStats.TotalHealth);

	OnStatChanged.Broadcast(EActorStatType::Health);
}

void UStatsComponent::SetCurrentStamina(const int& NewStamina)
{
	ActorStats.CurrentStamina = FMath::Clamp(NewStamina, 0, ActorStats.TotalStamina);

	OnStatChanged.Broadcast(EActorStatType::Stamina);
}

void UStatsComponent::SetTotalStamina(const int& NewStamina)
{
	ActorStats.TotalStamina = FMath::Clamp(NewStamina, MIN_TOTAL_STAT_VALUE, MAX_TOTAL_STAT_VALUE);

	// Ensure current stamina is in the limit 
	ActorStats.CurrentStamina = FMath::Clamp(NewStamina, 0, ActorStats.TotalStamina);

	OnStatChanged.Broadcast(EActorStatType::Stamina);
}

float UStatsComponent::GetHealthRatio()
{
	if (FMath::IsNearlyZero(static_cast<float>(ActorStats.TotalHealth)))
	{
		return 0.f;
	}

	return static_cast<float>(ActorStats.CurrentHealth) / ActorStats.TotalHealth;
}

float UStatsComponent::GetStaminaRatio()
{
	if (FMath::IsNearlyZero(static_cast<float>(ActorStats.TotalStamina)))
	{
		return 0.f;
	}

	return static_cast<float>(ActorStats.CurrentStamina) / ActorStats.TotalStamina;
}

