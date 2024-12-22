// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CustomDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UCustomDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UCustomDragDropOperation();

	UPROPERTY()
	TObjectPtr<class UItemSlot> ItemSlotRef;

	FVector2D DragOffset;

};
