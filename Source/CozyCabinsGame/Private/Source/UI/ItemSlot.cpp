// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Source/UI/CustomDragDropOperation.h>
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Source/UI/ItemSlotDragPreview.h"
#include <Source/Components/InventoryComponent.h>
#include "Components/Button.h"
#include <Source/Items/Item.h>
#include <Source/Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>

void UItemSlot::ClearSlot(UInventoryComponent& InventoryComp)
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(nullptr);
	Quantity->SetText(FText::GetEmpty());
	Name->SetText(FText::GetEmpty());
	InventoryIdx = -1;
	bIsOccupied = false;
	ItemId = EMPTY_GUID;
	ItemQty = -1;
	InventoryCompRef = &InventoryComp;
}

void UItemSlot::SetSlotData(const FItemData& ItemData, const int& Amount, const int& IdxInInventory, UInventoryComponent& InventoryComp)
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);
	CHECK(ItemButton);

	ItemQty = Amount;

	ItemImage->SetBrushFromTexture(ItemData.Icon);
	Quantity->SetText(FText::AsNumber(ItemQty));
	Name->SetText(FText::FromString(ItemData.Name));

	ItemImage->SetVisibility(ESlateVisibility::Visible);
	Quantity->SetVisibility(ESlateVisibility::Visible);
	Name->SetVisibility(ESlateVisibility::Visible);

	InventoryIdx = IdxInInventory;
	bIsOccupied = true;
	ItemId = ItemData.Id;
	InventoryCompRef = &InventoryComp;
}

void UItemSlot::SetEmptySlot(const int& IdxInInventory, UInventoryComponent& InventoryComp)
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(nullptr);
	Quantity->SetText(FText::GetEmpty());
	Name->SetText(FText::GetEmpty());

	ItemImage->SetVisibility(ESlateVisibility::Hidden);
	Quantity->SetVisibility(ESlateVisibility::Hidden);
	Name->SetVisibility(ESlateVisibility::Hidden);

	InventoryIdx = IdxInInventory;
	bIsOccupied = false;
	ItemId = EMPTY_GUID;
	ItemQty = -1;
	InventoryCompRef = &InventoryComp;
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	CHECK(DragPreviewClass);

	//Create the drag/drop operation
	TObjectPtr<UCustomDragDropOperation> pOperation = NewObject<UCustomDragDropOperation>();
	CHECK(pOperation);

	// Create the preview widget
	TObjectPtr<APlayerController> pPlayer = GetWorld()->GetFirstPlayerController();
	CHECK(pPlayer);

	TObjectPtr<UItemSlotDragPreview> pDragPreviewWidget = CreateWidget<UItemSlotDragPreview>(pPlayer, DragPreviewClass);
	CHECK(pDragPreviewWidget);

	pDragPreviewWidget->SetPreviewSlotData(this->ItemId, this->ItemQty, InventoryIdx, *this->ItemImage);

	pOperation->DefaultDragVisual = pDragPreviewWidget;
	pOperation->ItemSlotPreviewRef = pDragPreviewWidget;
	pOperation->InventoryCompRef = InventoryCompRef;

	OutOperation = pOperation;
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("On Drag Detected."));
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	TObjectPtr<UCustomDragDropOperation> pOperation = Cast<UCustomDragDropOperation>(InOperation);

	// Operation should be a valid CustomDragDropOperation
	if (!pOperation || !pOperation->InventoryCompRef || !pOperation->ItemSlotPreviewRef)
	{
		return false;
	}

	// Target Slot should have a valid Inventory component reference
	if (!InventoryCompRef)
	{
		return false;
	}

	TObjectPtr<UInventoryComponent> pInventoryComp = pOperation->InventoryCompRef;
	int srcIdx = pOperation->ItemSlotPreviewRef->InventoryIdx;
	int targetIdx = InventoryIdx;

	return pInventoryComp->TryTransferSlots(InventoryCompRef, srcIdx, targetIdx);

}

FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsOccupied || !MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Preview Left Mouse Down Failed."));
		return FEventReply(false).NativeReply;
	}

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;

}

FReply UItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 1. Slot should have an item
	// 2. Right mouse button should be pressed
	if (!bIsOccupied || !InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Right Mouse Down Failed."));
		return FEventReply(false).NativeReply;
	}

	OnConsumeItem();
	return FEventReply(true).NativeReply;
}

void UItemSlot::OnConsumeItem()
{
	CHECK(InventoryCompRef);

	FItemData itemData;
	int itemQty;
	if (InventoryCompRef->TryGetItemAtIndex(InventoryIdx, itemData, itemQty))
	{
		if (itemData.IsConsumable && itemData.ItemClass)
		{
			// Defer spawn the actor, call consume, then delete the actor instead of fully spawning it
			TObjectPtr<AItem> pItem = GetWorld()->SpawnActorDeferred<AItem>(itemData.ItemClass, FTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			CHECK(pItem);
			pItem->SetHidden(true);

			// Use the item
			TObjectPtr<APlayerController> pController = GetWorld()->GetFirstPlayerController();
			CHECK(pController);

			if (pItem->Execute_OnUse(pItem, GetWorld(), Cast<APlayerCharacter>(pController->GetOwner())))
			{
				UGameplayStatics::FinishSpawningActor(pItem, FTransform());
				// Now remove the item from the inventory, only 1 item in the stack
				InventoryCompRef->TryRemoveAtIndex(itemData.Id, InventoryIdx, 1);

				// remove the item after on use
				pItem->Destroy();
			}
		}
	}
}
