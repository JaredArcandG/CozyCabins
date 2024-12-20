// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Source/Items/ItemData.h>
#include "Source/UI/DraggableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemSlot.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API UItemSlot : public UDraggableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Quantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SlotBackgroundimage;

	// Reference to where in the inventory the slot is located
	int InventoryIdx;

public:

	void ClearSlot();

	void SetSlotData(const FItemData& ItemData, const int& Amount, const int& IdxInInventory);
	
};
