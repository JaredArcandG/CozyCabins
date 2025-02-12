// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Notifications/ItemNotificationWidget.h"
#include "Components/Image.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Source/Items/Item.h"
#include <Source/Notifications/ItemNotification.h>

/// <summary>
/// Override: Sets up the notification widget with the appropriate data
/// </summary>
/// <param name="Notification"></param>
void UItemNotificationWidget::SetupNotification(UBaseNotification& Notification)
{
	TObjectPtr<UItemNotification> pItemNotification = Cast<UItemNotification>(&Notification);
	CHECK(pItemNotification);
	CHECK(pItemNotification->ItemTexture);
	
	ItemImage->SetBrushFromTexture(pItemNotification->ItemTexture);
	
	FString sItemMessage = FString();
	
	switch (pItemNotification->ItemAction)
	{
		case EItemAction::AddItem:
			sItemMessage = ItemAddString.Replace(*ITEM_NAME_STR, *pItemNotification->ItemName.ToString());
			sItemMessage = sItemMessage.Replace(*ITEM_QTY_STR, *FString::FormatAsNumber(pItemNotification->Quantity));
			NotificationMessage->SetText(FText::FromString(sItemMessage));
			break;
		case EItemAction::RemoveItem:
			sItemMessage = ItemRemoveString.Replace(*ITEM_NAME_STR, *pItemNotification->ItemName.ToString());
			sItemMessage = sItemMessage.Replace(*ITEM_QTY_STR, *FString::FormatAsNumber(pItemNotification->Quantity));
			NotificationMessage->SetText(FText::FromString(sItemMessage));
			break;
		default:
			break;
	}
	
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
