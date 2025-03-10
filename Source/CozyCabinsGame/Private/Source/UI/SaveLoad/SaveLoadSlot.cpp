// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/SaveLoad/SaveLoadSlot.h"
#include <Source/Utils/CheckUtils.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>

void USaveLoadSlot::NativeConstruct()
{
	Super::NativeConstruct();
	SlotIdx = -1;

	CHECK(SlotButton);

	SlotButton->OnClicked.AddUniqueDynamic(this, &USaveLoadSlot::ExecuteSlotClicked);
	SlotButton->SetVisibility(ESlateVisibility::Visible);

}

void USaveLoadSlot::SetSlotData(class UTexture2D& InImage, const FText& InSaveSlotName, const FText& InSaveTimeText, const int& InSlotIdx)
{
	CHECK(GameImage);
	CHECK(SaveTimeText);
	CHECK(SlotNameText);
	CHECK(SlotButton);

	SlotButton->SetIsEnabled(true);


	GameImage->SetBrushFromTexture(&InImage);
	SaveTimeText->SetText(InSaveTimeText);
	SlotNameText->SetText(InSaveSlotName);
	SlotIdx = InSlotIdx;
}

void USaveLoadSlot::ClearSlot()
{
	CHECK(GameImage);
	CHECK(SaveTimeText);

	GameImage->SetBrushFromTexture(nullptr);
	SaveTimeText->SetText(FText::GetEmpty());
	SlotNameText->SetText(FText::GetEmpty());
	SlotIdx = -1;
}

void USaveLoadSlot::ExecuteSlotClicked()
{
	OnSlotClicked.Broadcast(SlotIdx);
}
