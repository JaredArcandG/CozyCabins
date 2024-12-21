// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/UI/DraggableWidget.h"
#include <Source/UI/CustomDragDropOperation.h>
#include <Source/Utils/CheckUtils.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UDraggableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OriginalLocation = FVector2f::Zero();
}

FReply UDraggableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OriginalLocation = InGeometry.AbsolutePosition;

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) 
	{ 
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply; 
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDraggableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UDraggableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
