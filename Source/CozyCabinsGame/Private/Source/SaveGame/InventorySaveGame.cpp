// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/InventorySaveGame.h"
#include <Source/Components/InventoryComponent.h>
#include <Source/Utils/CheckUtils.h>


UInventorySaveGame::UInventorySaveGame()
{
	MaxInventorySize = 0;
	MaxItemStackSize = 0;
	bCanUseInventory = false;
	CurrentSize = 0;
}

void UInventorySaveGame::OnSave(UObject& WorldContextObject, UObject& ObjectToSave)
{
	TObjectPtr<UInventoryComponent> pInventoryComp = Cast<UInventoryComponent>(&ObjectToSave);
	CHECK(pInventoryComp);

	MaxInventorySize = pInventoryComp->MaxInventorySize;
	MaxItemStackSize = pInventoryComp->MaxItemStackSize;
	bCanUseInventory = pInventoryComp->bCanUseInventory;
	CurrentSize = pInventoryComp->GetCurrentSize();
	ItemQuantityArr = pInventoryComp->GetItemQtyArray();
	ItemIdArr = pInventoryComp->GetItemIdArray();
}

void UInventorySaveGame::OnLoad(UObject& WorldContextObject, UObject& ObjectToLoad)
{
	TObjectPtr<UInventoryComponent> pInventoryComp = Cast<UInventoryComponent>(&ObjectToLoad);
	CHECK(pInventoryComp);

	pInventoryComp->MaxInventorySize = MaxInventorySize;
	pInventoryComp->MaxItemStackSize = MaxItemStackSize;
	pInventoryComp->bCanUseInventory = bCanUseInventory;
	pInventoryComp->SetCurrentSize(CurrentSize);
	pInventoryComp->SetItemQtyArray(ItemQuantityArr);
	pInventoryComp->SetItemIdArray(ItemIdArr);
}
