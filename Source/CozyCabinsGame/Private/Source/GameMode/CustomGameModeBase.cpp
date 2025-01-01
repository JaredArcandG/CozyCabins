// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/GameMode/CustomGameModeBase.h"
#include "Engine/DataTable.h"
#include "Source/Time/GameTimeManager.h"

ACustomGameModeBase::ACustomGameModeBase()
{
	ItemsDataTable = CreateDefaultSubobject<UDataTable>(TEXT("ItemsDataTable"));
	GameTimeManager = CreateDefaultSubobject<UGameTimeManager>(TEXT("GameTimeManager"));
	DroppedItemTimeToLiveSeconds = 0.0f;
}
