// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldDroppableItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UWorldDroppableItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<class UInteractableComponent> PlayerInteractableCompRef;

	UPROPERTY()
	TObjectPtr<class APawn> PlayerRef;


public:

	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
