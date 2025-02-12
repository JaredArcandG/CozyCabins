// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Player/Controller/CustomPlayerController.h"
#include "Source/UI/HUD/StatsHUD.h"
#include <Source/Utils/CheckUtils.h>
#include <Source/Player/PlayerCharacter.h>
#include <Source/UI/Common/GlobalUIManager.h>
#include <Source/UI/Common/WorldDroppableItemWidget.h>
#include <Source/UI/Notifications/ItemNotificationContainer.h>

/// <summary>
/// ctor
/// </summary>
ACustomPlayerController::ACustomPlayerController()
{
	GlobalUIManager = CreateDefaultSubobject<UGlobalUIManager>(TEXT("GlobalUIManager"));
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CHECK(PlayerHUDWidgetClass);
	PlayerHUD = CreateWidget<UStatsHUD>(this, PlayerHUDWidgetClass);
	CHECK(PlayerHUD);

	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(GetPawn());
	CHECK(pPlayer);

	PlayerHUD->InitializeWidget(*pPlayer);
	CHECK(PlayerHUD);

	PlayerHUD->AddToViewport();

	CHECK(PlayerItemNotificationClass);
	ItemNotificationWidget = CreateWidget<UItemNotificationContainer>(this, PlayerItemNotificationClass);
	
	CHECK(ItemNotificationWidget);
	ItemNotificationWidget->AddToViewport();

}
