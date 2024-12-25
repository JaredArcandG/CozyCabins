// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/UI/DraggableWidget.h"
#include <Source/UI/CustomDragDropOperation.h>
#include <Source/Utils/CheckUtils.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

/// <summary>
/// ctor
/// </summary>
void UDraggableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OriginalLocation = FVector2f::Zero();
}

/// <summary>
/// Allows drag start functionality on the widget
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InMouseEvent"></param>
/// <returns></returns>
FReply UDraggableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OriginalLocation = InGeometry.AbsolutePosition;

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) 
	{ 
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply; 
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

/// <summary>
///  Allows drag detection on the widget
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InMouseEvent"></param>
/// <param name="OutOperation"></param>
void UDraggableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

/// <summary>
/// Allows drop on the widget
/// </summary>
/// <param name="InGeometry"></param>
/// <param name="InDragDropEvent"></param>
/// <param name="InOperation"></param>
/// <returns></returns>
bool UDraggableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
