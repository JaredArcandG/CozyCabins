// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/StatsHUD.h"
#include "Source/Player/PlayerCharacter.h"
#include "Source/Components/StatsComponent.h"
#include "Components/ProgressBar.h"
#include <Source/Utils/CheckUtils.h>

void UStatsHUD::InitializeWidget(APlayerCharacter& PlayerCharacter)
{
	ActorStatsRef = PlayerCharacter.GetStatsComponent();

	CHECK(ActorStatsRef);

	ActorStatsRef->OnStatChanged.AddDynamic(this, &UStatsHUD::HandleStatsUpdate);

	CHECK(HealthBar);
	HealthBar->SetPercent(ActorStatsRef->GetHealthRatio());

	CHECK(StaminaBar);
	StaminaBar->SetPercent(ActorStatsRef->GetStaminaRatio());
}

void UStatsHUD::HandleStatsUpdate(EActorStatType UpdatedStat)
{
	CHECK(ActorStatsRef);

	switch (UpdatedStat)
	{
		case EActorStatType::Stamina:
			UpdateStaminaBar(ActorStatsRef->GetStaminaRatio());
			break;
		case EActorStatType::Health:
			UpdateHealthBar(ActorStatsRef->GetHealthRatio());
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Updated Stat is not supported by the UStatsHUD widget."));
	}
}

void UStatsHUD::UpdateHealthBar(float HealthPercent)
{
	CHECK(HealthBar);

	HealthBar->SetPercent(HealthPercent);
}

void UStatsHUD::UpdateStaminaBar(float StaminaPercent)
{
	StaminaBar->SetPercent(StaminaPercent);
}
