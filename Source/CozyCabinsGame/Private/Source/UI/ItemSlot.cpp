// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ItemSlot.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include <Source/UI/CustomDragDropOperation.h>
#include "Blueprint/WidgetBlueprintLibrary.h"

void UItemSlot::ClearSlot()
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(nullptr);
	Quantity->SetText(FText::GetEmpty());
	Name->SetText(FText::GetEmpty());
	InventoryIdx = -1;
	bIsOccupied = false;
}

void UItemSlot::SetSlotData(const FItemData& ItemData, const int& Amount, const int& IdxInInventory)
{
	CHECK(ItemImage);
	CHECK(Quantity);
	CHECK(Name);

	ItemImage->SetBrushFromTexture(ItemData.Icon);
	Quantity->SetText(FText::AsNumber(Amount));
	Name->SetText(FText::FromString(ItemData.Name));

	ItemImage->SetVisibility(ESlateVisibility::Visible);
	Quantity->SetVisibility(ESlateVisibility::Visible);
	Name->SetVisibility(ESlateVisibility::Visible);

	InventoryIdx = IdxInInventory;
	bIsOccupied = true;
}

void UItemSlot::SetEmptySlot(const int& IdxInInventory)
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
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	TObjectPtr<UCustomDragDropOperation> pOperation = NewObject<UCustomDragDropOperation>();
	CHECK(pOperation);

	TObjectPtr<UItemSlot> pSlotCopy = CreateWidget<UItemSlot>(this, UItemSlot::StaticClass());
	CHECK(pSlotCopy);

	pSlotCopy->ItemImage = this->ItemImage;
	pSlotCopy->Quantity = this->Quantity;
	pSlotCopy->Name = this->Name;
	pSlotCopy->InventoryIdx = this->InventoryIdx;
	pOperation->ItemSlotRef = pSlotCopy;

	pOperation->DefaultDragVisual = pSlotCopy;

	OutOperation = pOperation;

	pSlotCopy->AddToViewport();

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("On Drag Detected."));
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!bIsOccupied || !MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		UE_LOG(LogTemp, Warning, TEXT("Preview Mouse Down Failed."));
		return FEventReply(false).NativeReply;
	}

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton).NativeReply;

}
