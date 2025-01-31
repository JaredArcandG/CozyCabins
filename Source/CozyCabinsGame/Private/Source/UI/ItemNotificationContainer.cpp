// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemNotificationContainer.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/VerticalBox.h"
#include <Source/Player/PlayerCharacter.h>
#include <Source/Player/Controller/CustomPlayerController.h>
#include <Source/Components/PlayerInventoryComponent.h>
#include <Source/UI/ItemNotificationWidget.h>

void UItemNotificationContainer::NativeConstruct()
{
	PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECK(PlayerController);

	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());
	CHECK(pPlayer);

	TObjectPtr<UPlayerInventoryComponent> pInventoryComp = pPlayer->GetInventoryComponent();
	CHECK(pInventoryComp);

	pInventoryComp->OnPlayerInventoryChange.AddUniqueDynamic(this, &UItemNotificationContainer::OnItemNotificationReceived);
}

void UItemNotificationContainer::AddNotificationToContainer(UUserWidget& ChildToAdd)
{
	CHECK(NotificationContainer);

	NotificationContainer->AddChildToVerticalBox(&ChildToAdd);
}

void UItemNotificationContainer::OnItemNotificationReceived(FItemNotification ItemNotification)
{
	TObjectPtr<UItemNotificationWidget> pNotification = CreateWidget<UItemNotificationWidget>(this, ItemNotificationClass);
	CHECK(pNotification);

	pNotification->Setup(ItemNotification);

	AddNotificationToContainer(*pNotification);
}
