// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemNotificationWidget.h"
#include "Components/Image.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UItemNotificationWidget::Setup(const UImage& Image, const FText& Message)
{
	CHECK(ItemImage);
	CHECK(NotificationMessage);

	ItemImage->SetBrush(Image.GetBrush());
	NotificationMessage->SetText(Message);

	// Create a timer for how long the item notification should be live
	GetWorld()->GetTimerManager().SetTimer(hTimeActive, this, &UItemNotificationWidget::OnEndTimeActive, fTimeActive, false);
}

/// <summary>
/// Start the fade animation
/// </summary>
void UItemNotificationWidget::OnEndTimeActive()
{
	// Bind end function and Start the fade out animation
	CHECK(FadeOut);
	FWidgetAnimationDynamicEvent OnEndFadeOutEvent;
	OnEndFadeOutEvent.BindUFunction(this, FName("OnEndFadeOut"));
	BindToAnimationFinished(FadeOut, OnEndFadeOutEvent);
	PlayAnimation(FadeOut);
}

void UItemNotificationWidget::OnEndFadeOut()
{
	// Destroy the widget
	RemoveFromParent();
	this->ConditionalBeginDestroy();
}
