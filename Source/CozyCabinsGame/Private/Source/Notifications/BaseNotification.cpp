// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Notifications/BaseNotification.h"

TSubclassOf<UUserWidget> UBaseNotification::GetWidgetNotificationClass()
{
	return WidgetNotificationClass;
}

void UBaseNotification::Setup(TSubclassOf<UUserWidget> InWidgetClass)
{
	WidgetNotificationClass = InWidgetClass;
}
