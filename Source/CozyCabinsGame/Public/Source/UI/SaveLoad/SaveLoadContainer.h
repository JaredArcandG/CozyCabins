// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveLoadContainer.generated.h"

UENUM()
enum class ESaveLoadMode : uint8
{
	None,
	Save,
	Load
};

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API USaveLoadContainer : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UScrollBox> SaveLoadSlotContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USaveLoadSlot> SaveLoadSlotClass;

	ESaveLoadMode SlotMode;

protected:

	UPROPERTY()
	TObjectPtr<class UCustomGameInstance> CustomGameInstance;

public:

	virtual void NativeConstruct() override;

	void UpdateSlots();

	void SetSlotMode(const ESaveLoadMode& InSlotMode);

private: 

	UFUNCTION()
	void OnSlotClicked(int SlotIdx);
	
};
