// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/InventorySaveGame.h"
#include <Source/Components/InventoryComponent.h>
#include <Source/Utils/CheckUtils.h>


UInventorySaveGame::UInventorySaveGame()
{

}

void UInventorySaveGame::OnSaveUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToSave)
{
	Super::OnSaveUnique(WorldContextObject, ObjId, ObjectToSave);

	FInventorySaveGameData SaveData;

	TObjectPtr<UInventoryComponent> pInventoryComp = Cast<UInventoryComponent>(&ObjectToSave);
	CHECK(pInventoryComp);

	SaveData.MaxInventorySize = pInventoryComp->MaxInventorySize;
	SaveData.MaxItemStackSize = pInventoryComp->MaxItemStackSize;
	SaveData.bCanUseInventory = pInventoryComp->bCanUseInventory;
	SaveData.CurrentSize = pInventoryComp->GetCurrentSize();
	SaveData.ItemQuantityArr = pInventoryComp->GetItemQtyArray();
	SaveData.ItemIdArr = pInventoryComp->GetItemIdArray();

	TTuple<FGuid, FInventorySaveGameData> kvp = TTuple<FGuid, FInventorySaveGameData>();
	kvp.Key = ObjId;
	kvp.Value = SaveData;

	DataMap.Add(kvp);
}

void UInventorySaveGame::ClearOverwrite()
{
	Super::ClearOverwrite();

	DataMap.Empty();
}
