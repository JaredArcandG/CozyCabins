// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/PlayerStatsSaveGame.h"
#include <Source/Utils/CheckUtils.h>

UPlayerStatsSaveGame::UPlayerStatsSaveGame()
{
	HungerDecayRate = 0;
	HungerDecayGametimeMins = 0;
	TimeSinceLastHungerDecay = FDateTime::UtcNow();
}

void UPlayerStatsSaveGame::OnSave(const UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<UStatsComponent> pStats = Cast<UStatsComponent>(&ObjectToSave);
	CHECK(pStats);

	HungerDecayRate = pStats->GetHungerDecayRate();
	HungerDecayGametimeMins = pStats->GetHungerDecayGameTimeMins();
	ActorStats.CurrentHealth = pStats->GetCurrentHealth();
	ActorStats.TotalHealth = pStats->GetTotalHealth();
	ActorStats.CurrentHunger = pStats->GetCurrentHunger();
	ActorStats.TotalHunger = pStats->GetTotalHunger();
	ActorStats.CurrentStamina = pStats->GetCurrentStamina();
	ActorStats.TotalStamina = pStats->GetTotalStamina();
}

void UPlayerStatsSaveGame::OnLoad(const UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<UStatsComponent> pStats = Cast<UStatsComponent>(&ObjectToLoad);
	CHECK(pStats);

	pStats->SetHungerDecayRate(HungerDecayRate);
	pStats->SetHungerDecayGameTimeMins(HungerDecayGametimeMins);
	pStats->SetCurrentHealth(ActorStats.CurrentHealth);
	pStats->SetTotalHealth(ActorStats.TotalHealth);
	pStats->SetCurrentHunger(ActorStats.CurrentHunger);
	pStats->SetTotalHunger(ActorStats.TotalHunger);
	pStats->SetCurrentStamina(ActorStats.CurrentStamina);
	pStats->SetTotalStamina(ActorStats.TotalStamina);
}
