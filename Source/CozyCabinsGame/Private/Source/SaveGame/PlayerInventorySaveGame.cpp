// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/PlayerInventorySaveGame.h"
#include "GameFramework/PlayerController.h"
#include "Source/Player/PlayerCharacter.h"
#include "Source/Components//PlayerInventoryComponent.h"
#include <Source/Utils/CheckUtils.h>

/// <summary>
/// ctor
/// </summary>
UPlayerInventorySaveGame::UPlayerInventorySaveGame()
{
	MaxInventorySize = 0;
	MaxItemStackSize = 0;
	bCanUseInventory = false;
	CurrentSize = 0;
}

void UPlayerInventorySaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
	Super::OnSave(WorldContextObject, ObjectToSave);

	TObjectPtr<UPlayerInventoryComponent> pPlayerInventory = Cast<UPlayerInventoryComponent>(&ObjectToSave);
	CHECK(pPlayerInventory);

	MaxInventorySize = pPlayerInventory->MaxInventorySize;
	MaxItemStackSize = pPlayerInventory->MaxItemStackSize;
	bCanUseInventory = pPlayerInventory->bCanUseInventory;
	CurrentSize = pPlayerInventory->GetCurrentSize();
	ItemQuantityArr = pPlayerInventory->GetItemQtyArray();
	ItemIdArr = pPlayerInventory->GetItemIdArray();
}

void UPlayerInventorySaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToSave)
{
	Super::OnLoad(WorldContextObject, ObjectToSave);

	TObjectPtr<UPlayerInventoryComponent> pPlayerInventory = Cast<UPlayerInventoryComponent>(&ObjectToSave);
	CHECK(pPlayerInventory);

	pPlayerInventory->MaxInventorySize = MaxInventorySize;
	pPlayerInventory->MaxItemStackSize = MaxItemStackSize;
	pPlayerInventory->bCanUseInventory = bCanUseInventory;
	pPlayerInventory->SetCurrentSize(CurrentSize);
	pPlayerInventory->SetItemQtyArray(ItemQuantityArr);
	pPlayerInventory->SetItemIdArray(ItemIdArr);
}

