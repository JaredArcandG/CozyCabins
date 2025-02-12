// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GlobalUIManager.generated.h"

/**
 * Keeps track of global UI operation to for any global states that need to be kept track of
 */
UCLASS()
class COZYCABINSGAME_API UGlobalUIManager : public UObject
{
	GENERATED_BODY()

public:

	UGlobalUIManager();

	UPROPERTY()
	TObjectPtr<class UUserWidget> DragWidgetRef;

	UPROPERTY()
	TObjectPtr<class UUserWidget> DragSlotRef;

protected:

	bool bIsActiveDragState;

public:

	bool IsActiveDragState();

	void SetIsActiveDragState(const bool& NewState, TObjectPtr<class UUserWidget> WidgetRef, TObjectPtr<class UUserWidget> SlotRef);
	
};
