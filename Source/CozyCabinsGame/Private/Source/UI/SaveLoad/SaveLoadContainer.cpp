// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/SaveLoad/SaveLoadContainer.h"
#include "Source/UI/SaveLoad/SaveLoadSlot.h"
#include <Source/Utils/CheckUtils.h>
#include <Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>
#include <Source/GameInstance/CustomGameInstance.h>
#include <Source/SaveGame/SaveGameSlot.h>
#include <Components/Button.h>

void USaveLoadContainer::NativeConstruct()
{
	Super::NativeConstruct();

	CHECK(SaveLoadSlotContainer);
	CHECK(SaveLoadSlotClass);

	CustomGameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CHECK(CustomGameInstance);

	SlotMode = ESaveLoadMode::None;

	int slotIdx = 0;

	for (auto& pSlot : CustomGameInstance->GetGameSlots())
	{
		if (pSlot)
		{
			// Create SaveLoadSlot, then add to container
			TObjectPtr<USaveLoadSlot> pSaveSlotUI = CreateWidget<USaveLoadSlot>(this, SaveLoadSlotClass);
			
			if (pSaveSlotUI)
			{
				TObjectPtr<UTexture2D> pTexture = NewObject<UTexture2D>();
				CHECK(pTexture);

				pSaveSlotUI->SetSlotData(*pTexture, FText::FromString(pSlot->GetBaseSaveName()), slotIdx);
				slotIdx++;

				SaveLoadSlotContainer->AddChild(pSaveSlotUI);
				
			}
		}
	}
}

void USaveLoadContainer::UpdateSlots()
{
	for (int i = 0; i < SaveLoadSlotContainer->GetChildrenCount(); i++)
	{
		TObjectPtr<USaveLoadSlot> pSaveSlotUI = Cast<USaveLoadSlot>(SaveLoadSlotContainer->GetChildAt(i));

		if (pSaveSlotUI && CustomGameInstance->GetGameSlots().IsValidIndex(i))
		{
			TObjectPtr<USaveGameSlot> pSaveGameSlot = CustomGameInstance->GetGameSlots()[i];

			if (pSaveGameSlot)
			{
				TObjectPtr<UTexture2D> pTexture = NewObject<UTexture2D>();
				CHECK(pTexture);

				pSaveSlotUI->SetSlotData(*pTexture, FText::FromString(pSaveGameSlot->GetBaseSaveName()), i);
			}
		}
	}
}

void USaveLoadContainer::SetSlotMode(const ESaveLoadMode& InSlotMode)
{
	SlotMode = InSlotMode;
}

void USaveLoadContainer::OnSlotClicked(int SlotIdx)
{
	CHECK(CustomGameInstance);

	switch (SlotMode)
	{
		case ESaveLoadMode::Load:
			CustomGameInstance->OnLoadGame(SlotIdx);
			this->SetVisibility(ESlateVisibility::Hidden);
			break;
		case ESaveLoadMode::Save:
			CustomGameInstance->OnSaveGame(SlotIdx);
			this->SetVisibility(ESlateVisibility::Hidden);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("%hs: Specified SaveLoadMode is unsupported."), __FUNCTION__);
	}
}
