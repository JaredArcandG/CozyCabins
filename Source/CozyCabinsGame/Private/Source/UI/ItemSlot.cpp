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
#include <Components/ScaleBoxSlot.h>
#include "Components/Sizebox.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Source/Player/Controller/CustomPlayerController.h>
#include <Source/UI/GlobalUIManager.h>
#include <Source/Components/InteractableComponent.h>
#include <Source/Items/Item.h>

/// <summary>
/// Ctor
/// </summary>
void UItemSlot::NativeConstruct()
{
	InventoryIdx = -1;
	bIsOccupied = false;
	ItemId = EMPTY_GUID;
	ItemQty = -1;
	InventoryCompRef = nullptr;
	PreviewQtyToTransfer = 0;
	DragPreviewWidget = nullptr;
	ItemClass = nullptr;

	PlayerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	CHECK(PlayerController);

	GlobalUIManager = PlayerController->GlobalUIManager;

	TObjectPtr<APlayerCharacter> pPlayer = Cast<APlayerCharacter>(PlayerController->GetPawn());
	CHECK(pPlayer);

	InteractableComp = pPlayer->GetInteractionComponent();

}

/// <summary>
/// Removes previously set slot values
/// </summary>
/// <param name="InventoryComp"></param>
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
	PreviewQtyToTransfer = 0;
	ItemClass = nullptr;
}

/// <summary>
/// Sets the slot data to the given parameters
/// </summary>
/// <param name="ItemData"></param>
/// <param name="Amount"></param>
/// <param name="IdxInInventory"></param>
/// <param name="InventoryComp"></param>
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
	PreviewQtyToTransfer = 0;
	ItemClass = ItemData.ItemClass;
}

/// <summary>
/// Empties a slot
/// </summary>
/// <param name="IdxInInventory"></param>
/// <param name="InventoryComp"></param>
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
	PreviewQtyToTransfer = 0;
}

/// <summary>
/// Called when a slot drag is detected
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InMouseEvent"></param>
/// <param name="OutOperation"></param>
void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	CHECK(DragPreviewClass);

	// Remove the drag preview widget since the drop operation was invoked, as well as the operation reference
	RemovePreviewWidgetPreDrag();

	//Create the drag/drop operation
	TObjectPtr<UCustomDragDropOperation> pOperation = NewObject<UCustomDragDropOperation>();
	CHECK(pOperation);

	// Create the new preview widget
	TObjectPtr<UItemSlotDragPreview> pDragPreviewWidget = CreateWidget<UItemSlotDragPreview>(PlayerController, DragPreviewClass);
	CHECK(pDragPreviewWidget);
	
	pDragPreviewWidget->SetPreviewSlotData(this->ItemId, PreviewQtyToTransfer, InventoryIdx, this->ItemImage, this->ItemClass);

	pOperation->DefaultDragVisual = pDragPreviewWidget;
	pOperation->ItemSlotPreviewRef = pDragPreviewWidget;
	pOperation->InventoryCompRef = InventoryCompRef;

	OutOperation = pOperation; 
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	GlobalUIManager->SetIsActiveDragState(true, this, pDragPreviewWidget);

	UE_LOG(LogTemp, Warning, TEXT("On Drag Detected."));
}

/// <summary>
/// Called when a slot is dropped
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InDragDropEvent"></param>
/// <param name="InOperation"></param>
/// <returns></returns>
bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	TObjectPtr<UCustomDragDropOperation> pOperation = Cast<UCustomDragDropOperation>(InOperation);

	// Operation should be a valid CustomDragDropOperation
	if (!pOperation || !pOperation->InventoryCompRef || !pOperation->ItemSlotPreviewRef)
	{
		return false;
	}

	TObjectPtr<UInventoryComponent> pInventoryComp = pOperation->InventoryCompRef;
	int srcIdx = pOperation->ItemSlotPreviewRef->InventoryIdx;
	int srcQty = pOperation->ItemSlotPreviewRef->Quantity;
	int targetIdx = InventoryIdx;

	// Drop operation will occur - set preview quantity back to zero, no active drag
	PreviewQtyToTransfer = 0;
	GlobalUIManager->SetIsActiveDragState(false, nullptr, nullptr);

	return pInventoryComp->TryTransferSlotsWithQuantity(InventoryCompRef, srcIdx, srcQty, targetIdx);

}

/// <summary>
/// Called when the mouse button is down
/// Starts the drag process
/// </summary>
/// <param name="MyGeometry"></param>
/// <param name="MouseEvent"></param>
/// <returns></returns>
FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsOccupied || !MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Preview Left Mouse Down Failed."));
		return FEventReply(false).NativeReply;
	}

	// Full slot drag, preview quantity is the entire slot's quantity
	PreviewQtyToTransfer = this->ItemQty;

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;

}

/// <summary>
/// Called when the right mouse button is down
/// Starts the item consumption process
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InMouseEvent"></param>
/// <returns></returns>
FReply UItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 1. Slot should have an item
	// 2. Right mouse button should be pressed
	// 3. Can't consume an item while actively dragging something n 
	if (!bIsOccupied || !InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) || GlobalUIManager->IsActiveDragState())
	{
		UE_LOG(LogTemp, Warning, TEXT("Right Mouse Down Failed."));
		return FEventReply(false).NativeReply;
	}

	OnConsumeItem();
	return FEventReply(true).NativeReply;
}

/// <summary>
/// Called when an item is consumed
/// TODO: Move core consumption logic to InventoryComponent
/// </summary>
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

/// <summary>
/// Called when the mouse wheel is moved over a slot
/// </summary>
/// <param name="MyGeometry"></param>
/// <param name="MouseEvent"></param>
/// <returns></returns>
FReply UItemSlot::NativeOnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsOccupied || !GlobalUIManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Mouse Wheel Failed - empty slot or no global manager."));
		return FEventReply(false).NativeReply;
	}

	// Make sure we are wheeling on a valid slot
	if (GlobalUIManager->IsActiveDragState() && !(GlobalUIManager->DragWidgetRef == this))
	{
		UE_LOG(LogTemp, Warning, TEXT("On Mouse Wheel Failed - wheeling on invalid slot."));
		return FEventReply(false).NativeReply;
	}

	int deltaQty = FMath::RoundToInt(MouseEvent.GetWheelDelta());
	PreviewQtyToTransfer = FMath::Clamp(PreviewQtyToTransfer + deltaQty, 1, this->ItemQty);

	UE_LOG(LogTemp, Warning, TEXT("Mouse Wheel Success - ActiveDragState: %hs"), GlobalUIManager->IsActiveDragState() ? "True" : "False");

	FEventReply targetReply = FEventReply(true);

	if (GlobalUIManager->IsActiveDragState())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostDrag Scenario"));

		TObjectPtr<UItemSlotDragPreview> pDragPreviewWidget = Cast<UItemSlotDragPreview>(GlobalUIManager->DragSlotRef);

		// Update the slot
		if (pDragPreviewWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Drag preview slot reference updated."));
			pDragPreviewWidget->SetPreviewSlotData(pDragPreviewWidget->ItemId, PreviewQtyToTransfer, pDragPreviewWidget->InventoryIdx, pDragPreviewWidget->ItemImage, pDragPreviewWidget->ItemClass);
		}
		
		return targetReply.NativeReply;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PreDrag Scenario"));

		// DragPreview Scenario on mouse wheel scroll
		// Create it if it doesn't exist, otherwise update it
		if (!DragPreviewWidget)
		{
			ShowPreviewWidgetPreDrag();

			UE_LOG(LogTemp, Warning, TEXT("On Mouse Wheel Start. Preview Created."));
		}
		else
		{
			UpdatePreviewWidgetPreDrag();
			UE_LOG(LogTemp, Warning, TEXT("On Mouse Wheel Update Preview. New Qty: %d"), PreviewQtyToTransfer);
		}

		return UWidgetBlueprintLibrary::DetectDrag(targetReply, this, EKeys::Mouse2D).NativeReply;
	}
}

/// <summary>
/// Shows the preview widget before a drag operation is perfomed
/// This is needed for the mouse wheel scenario
/// </summary>
void UItemSlot::ShowPreviewWidgetPreDrag()
{
	// Create the preview widget

	DragPreviewWidget = CreateWidget<UItemSlotDragPreview>(PlayerController, DragPreviewClass);
	CHECK(DragPreviewWidget);

	DragPreviewWidget->SetPreviewSlotData(this->ItemId, PreviewQtyToTransfer, InventoryIdx, this->ItemImage, this->ItemClass);

	CHECK(DragPreviewWidget->ItemSizeBox);

	FVector2D vDesiredSize = DragPreviewWidget->ItemSizeBox->GetDesiredSize();

	DragPreviewWidget->SetDesiredSizeInViewport(vDesiredSize); // Adjust the size as needed

	DragPreviewWidget->AddToViewport();

	// Get the current mouse position to ensure it's spawned where the mouse is
	FVector2D vMousePosition;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, vMousePosition.X, vMousePosition.Y);

	FVector2D vFinalPos = vMousePosition - (vDesiredSize * 0.5);

	DragPreviewWidget->SetPositionInViewport(vFinalPos, false);
}

/// <summary>
/// Updates the preview widget before a drag operation is perfomed
/// This is needed for the mouse wheel scenario
/// </summary>
void UItemSlot::UpdatePreviewWidgetPreDrag()
{
	CHECK(DragPreviewWidget);

	DragPreviewWidget->SetPreviewSlotData(this->ItemId, PreviewQtyToTransfer, InventoryIdx, this->ItemImage, this->ItemClass);
}

/// <summary>
/// Removes the preview slot widget when a drag operation is perfomed
/// This is needed for the mouse wheel scenario
/// </summary>
void UItemSlot::RemovePreviewWidgetPreDrag()
{
	CHECK(DragPreviewWidget);
	CHECK(GlobalUIManager);

	// Remove the global drag state since we are destroying or pre drag widget
	GlobalUIManager->SetIsActiveDragState(false, nullptr, nullptr);

	DragPreviewWidget->RemoveFromParent();
	DragPreviewWidget->ConditionalBeginDestroy();

	DragPreviewWidget = nullptr;
}
