// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/GameMode/CustomGameModeBase.h"
#include "Engine/DataTable.h"

ACustomGameModeBase::ACustomGameModeBase()
{
	ItemsDataTable = CreateDefaultSubobject<UDataTable>(TEXT("ItemsDataTable"));
}
