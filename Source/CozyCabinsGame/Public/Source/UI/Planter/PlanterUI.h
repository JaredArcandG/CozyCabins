// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlanterUI.generated.h"

class UItemSlotContainer;

UCLASS()
class COZYCABINSGAME_API UPlanterUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotContainer> PlayerInventory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlotContainer> PlanterInventory;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> PlayerInventoryComponent;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> PlanterInventoryComponent;

	UFUNCTION(BlueprintCallable)
	void InitUI(class UInventoryComponent* PlayerComponent, class UInventoryComponent* PlanterComponent);

	UFUNCTION(BlueprintCallable)
	void UpdateUI();

};
