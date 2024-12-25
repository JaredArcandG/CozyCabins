// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/GlobalUIManager.h"
#include "Blueprint/UserWidget.h"

/// <summary>
/// Ctor
/// </summary>
UGlobalUIManager::UGlobalUIManager()
{
	bIsActiveDragState = false;
}

/// <summary>
/// If some UI in game is being actively dragged
/// </summary>
/// <returns></returns>
bool UGlobalUIManager::IsActiveDragState()
{
	return bIsActiveDragState;
}

/// <summary>
/// Set the drag state by giving the widget reference and slot reference (for UI children)
/// </summary>
/// <param name="NewState"></param>
/// <param name="WidgetRef"></param>
/// <param name="SlotRef"></param>
void UGlobalUIManager::SetIsActiveDragState(const bool& NewState, TObjectPtr<UUserWidget> WidgetRef, TObjectPtr<class UUserWidget> SlotRef)
{
	bIsActiveDragState = NewState;
	DragWidgetRef = WidgetRef;
	DragSlotRef = SlotRef;
}
