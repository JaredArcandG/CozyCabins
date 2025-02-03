// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/Notifications/BaseNotification.h"
#include <Source/Items/Item.h>
#include "ItemNotification.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemNotification : public UBaseNotification
{
	GENERATED_BODY()

public:

	UPROPERTY()
	EItemAction ItemAction = EItemAction::None;

	UPROPERTY()
	TObjectPtr<class UTexture2D> ItemTexture = nullptr;

	UPROPERTY()
	int Quantity = -1;

	UPROPERTY()
	FText ItemName = FText::GetEmpty();

};
