// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <UObject/NoExportTypes.h>
#include "DraggableWidget.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class COZYCABINSGAME_API UDraggableWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FVector2f OriginalLocation;

public:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
