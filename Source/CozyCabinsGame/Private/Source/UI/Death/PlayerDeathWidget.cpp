// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Death/PlayerDeathWidget.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/Image.h"

void UPlayerDeathWidget::OnPlayerDeath(UObject& ObjectToBind, const FName& FunctionToBind)
{
	CHECK(OnDeath);
	FWidgetAnimationDynamicEvent OnDeathEvent;
	OnDeathEvent.BindUFunction(&ObjectToBind, FunctionToBind);
	BindToAnimationFinished(OnDeath, OnDeathEvent);
	PlayAnimation(OnDeath);
}

void UPlayerDeathWidget::ResetWidget()
{
	CHECK(DeathScreenImage);

	// Reset the death screen to what itw as originally
	float rgba[4] = { 1, 1, 1, 0 };
	FLinearColor color;
	color.R = rgba[0];
	color.B = rgba[1];
	color.G = rgba[2];
	color.A = rgba[3];
	DeathScreenImage->SetColorAndOpacity(color);
}
