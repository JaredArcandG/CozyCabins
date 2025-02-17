// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/SaveLoad/SaveLoadContainer.h"
#include "Source/UI/SaveLoad/SaveLoadSlot.h"
#include <Source/Utils/CheckUtils.h>
#include <Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>
#include <Source/GameInstance/CustomGameInstance.h>
#include <Source/SaveGame/SaveGameSlot.h>
#include <Components/Button.h>
#include <Source/SaveGame/PlayerCoreSaveGame.h>

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
			
			if (pSaveSlotUI && pSlot->PlayerCoreSaveGame)
			{
				TObjectPtr<UTexture2D> pTexture = pSlot->PlayerCoreSaveGame->GetGameImageTexture();

				pSaveSlotUI->SetSlotData(*pTexture, FText::FromString(pSlot->BaseSaveName), FText::FromString(pSlot->PlayerCoreSaveGame->GameTime), slotIdx);
				pSaveSlotUI->OnSlotClicked.AddUniqueDynamic(this, &USaveLoadContainer::OnSlotClicked);

				SaveLoadSlotContainer->AddChild(pSaveSlotUI);
				pSaveSlotUI->SetPadding(FMargin(10));
				
			}
		}

		slotIdx++;
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

			if (pSaveGameSlot && pSaveGameSlot->PlayerCoreSaveGame)
			{
				TObjectPtr<UTexture2D> pTexture = pSaveGameSlot->PlayerCoreSaveGame->GetGameImageTexture();

				pSaveSlotUI->SetSlotData(*pTexture, FText::FromString(pSaveGameSlot->BaseSaveName), FText::FromString(pSaveGameSlot->PlayerCoreSaveGame->GameTime), i);
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

	UpdateSlots();
}
