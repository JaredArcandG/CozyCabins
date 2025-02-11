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

void UInventorySaveGame::OnLoadUnique(const UObject& WorldContextObject, const FGuid& ObjId, UObject& ObjectToLoad)
{
	Super::OnLoadUnique(WorldContextObject, ObjId, ObjectToLoad);

	TObjectPtr<UInventoryComponent> pInventoryComp = Cast<UInventoryComponent>(&ObjectToLoad);
	CHECK(pInventoryComp);

	if (DataMap.Contains(ObjId))
	{
		FInventorySaveGameData SaveData = DataMap[ObjId];
		pInventoryComp->MaxInventorySize = SaveData.MaxInventorySize;
		pInventoryComp->MaxItemStackSize = SaveData.MaxItemStackSize;
		pInventoryComp->bCanUseInventory = SaveData.bCanUseInventory;
		pInventoryComp->SetCurrentSize(SaveData.CurrentSize);
		pInventoryComp->SetItemQtyArray(SaveData.ItemQuantityArr);
		pInventoryComp->SetItemIdArray(SaveData.ItemIdArr);

		// Broadcast the change so the UI is also aware
		pInventoryComp->OnInventoryChange.Broadcast();
	}
}

void UInventorySaveGame::ClearOverwrite()
{
	Super::ClearOverwrite();

	DataMap.Empty();
}
