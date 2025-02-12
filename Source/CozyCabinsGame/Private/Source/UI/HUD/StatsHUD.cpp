// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/HUD/StatsHUD.h"
#include "Source/Player/PlayerCharacter.h"
#include "Source/Components/StatsComponent.h"
#include "Components/ProgressBar.h"
#include <Source/Utils/CheckUtils.h>

/// <summary>
/// Called to setup the widget after ctor
/// </summary>
/// <param name="PlayerCharacter"></param>
void UStatsHUD::InitializeWidget(APlayerCharacter& PlayerCharacter)
{
	ActorStatsRef = PlayerCharacter.GetStatsComponent();

	CHECK(ActorStatsRef);

	ActorStatsRef->OnStatChanged.AddDynamic(this, &UStatsHUD::HandleStatsUpdate);

	CHECK(HealthBar);
	HealthBar->SetPercent(ActorStatsRef->GetHealthRatio());

	CHECK(StaminaBar);
	StaminaBar->SetPercent(ActorStatsRef->GetStaminaRatio());

	CHECK(HungerBar);
	HungerBar->SetPercent(ActorStatsRef->GetHungerRatio());
}

/// <summary>
/// Shows the widget in the viewport
/// </summary>
void UStatsHUD::HideWidget()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

/// <summary>
/// Hides the widget in the viewport
/// </summary>
void UStatsHUD::ShowWidget()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

/// <summary>
/// Dynamic function linked to the on stat changed variable
/// </summary>
/// <param name="UpdatedStat"></param>
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
		case EActorStatType::Hunger:
			UpdateHungerBar(ActorStatsRef->GetHungerRatio());
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Updated Stat is not supported by the UStatsHUD widget."));
	}
}

/// <summary>
/// Updates the health bar to the new percentage
/// </summary>
/// <param name="HealthPercent"></param>
void UStatsHUD::UpdateHealthBar(float HealthPercent)
{
	CHECK(HealthBar);

	HealthBar->SetPercent(HealthPercent);
}

/// <summary>
/// Updates the stamina bar to the new percentage
/// </summary>
/// <param name="HealthPercent"></param>
void UStatsHUD::UpdateStaminaBar(float StaminaPercent)
{
	CHECK(StaminaBar);

	StaminaBar->SetPercent(StaminaPercent);
}

/// <summary>
/// Updates the hunger bar to the new percentage
/// </summary>
/// <param name="HealthPercent"></param>
void UStatsHUD::UpdateHungerBar(float HungerPercent)
{
	CHECK(HungerBar);

	HungerBar->SetPercent(HungerPercent);
}
