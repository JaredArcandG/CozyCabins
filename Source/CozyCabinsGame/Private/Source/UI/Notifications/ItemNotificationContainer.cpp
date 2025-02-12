// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Notifications/ItemNotificationContainer.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/VerticalBox.h"
#include <Source/Player/PlayerCharacter.h>
#include <Source/Player/Controller/CustomPlayerController.h>
#include <Source/Components/PlayerInventoryComponent.h>
#include <Source/UI/Notifications/ItemNotificationWidget.h>
#include "Source/Notifications/BaseNotification.h"

void UItemNotificationContainer::NativeConstruct()
{
	PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECK(PlayerController);

	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());
	CHECK(pPlayer);

	TObjectPtr<UPlayerInventoryComponent> pInventoryComp = pPlayer->GetInventoryComponent();
	CHECK(pInventoryComp);

	// Add all notifications hooks here
	pInventoryComp->OnPlayerInventoryChange.AddUniqueDynamic(this, &UItemNotificationContainer::OnNotificationReceived);
}

void UItemNotificationContainer::AddNotificationToContainer(UUserWidget& ChildToAdd)
{
	CHECK(NotificationContainer);

	NotificationContainer->AddChildToVerticalBox(&ChildToAdd);
}

void UItemNotificationContainer::OnNotificationReceived(UBaseNotification* Notification)
{
	CHECK(Notification);
	CHECK(Notification->GetWidgetNotificationClass());

	TObjectPtr<UUserWidget> pNotificationWidget = CreateWidget<UUserWidget>(this, Notification->GetWidgetNotificationClass());
	CHECK(pNotificationWidget);

	if (auto pINotification = Cast<INotification>(pNotificationWidget))
	{
		pINotification->SetupNotification(*Notification);
		AddNotificationToContainer(*pNotificationWidget);
	}

}
