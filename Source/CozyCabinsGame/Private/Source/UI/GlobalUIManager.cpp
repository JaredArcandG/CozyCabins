// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/GlobalUIManager.h"
#include "Blueprint/UserWidget.h"

UGlobalUIManager::UGlobalUIManager()
{
	bIsActiveDragState = false;
}

bool UGlobalUIManager::IsActiveDragState()
{
	return bIsActiveDragState;
}

void UGlobalUIManager::SetIsActiveDragState(const bool& NewState, TObjectPtr<UUserWidget> WidgetRef, TObjectPtr<class UUserWidget> SlotRef)
{
	bIsActiveDragState = NewState;
	DragWidgetRef = WidgetRef;
	DragSlotRef = SlotRef;
}
