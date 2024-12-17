// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/DraggableWidget.h"
#include <Source/UI/CustomDragDropOperation.h>
#include <Source/Utils/CheckUtils.h>

void UDraggableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OriginalLocation = FVector2f::Zero();
}

void UDraggableWidget::OnDragStart(const FGeometry& Geometry, const FPointerEvent& PointerEvent, UDragDropOperation*& OutOperation)
{
	TObjectPtr<UCustomDragDropOperation> pOperation = NewObject<UCustomDragDropOperation>();
	CHECK(pOperation);

	OriginalLocation = Geometry.AbsolutePosition;

	pOperation->WidgetRef = this;
	pOperation->Pivot = EDragPivot::MouseDown;
	pOperation->DragOffset = Geometry.AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());

	OutOperation = pOperation;
}

bool UDraggableWidget::OnDrop(const FGeometry& Geometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
	if (TObjectPtr<UCustomDragDropOperation> pOperation = Cast<UCustomDragDropOperation>(Operation))
	{
		if (pOperation->WidgetRef)
		{
			// Calculate the position of the slot closest to where it will try to drop (also add maximum distance so it doesn't try to drop too far off)
			// If the slot space is available, swap the slot information, if not, then "cancel" the operation by moving the slot to its original position
		}
	}

	return false;
}

void UDraggableWidget::OnDragOver(const FGeometry& Geometry, const FDragDropEvent& DragDropEvent, UDragDropOperation* Operation)
{
}
