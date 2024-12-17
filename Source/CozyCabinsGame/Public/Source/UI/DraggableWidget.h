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

	void OnDragStart(const FGeometry& Geometry, const FPointerEvent& PointerEvent, UDragDropOperation*& OutOperation);
	
	bool OnDrop(const FGeometry& Geometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation);

	void OnDragOver(const FGeometry& Geometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation);
};
