// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemNotificationContainer.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/VerticalBox.h"

void UItemNotificationContainer::NativeConstruct()
{
	PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
}

void UItemNotificationContainer::AddNotificationToContainer(UUserWidget& ChildToAdd)
{
	CHECK(NotificationContainer);

	NotificationContainer->AddChildToVerticalBox(&ChildToAdd);
}
