// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/Planter/PlanterUI.h"
#include "Source/UI/Items/ItemSlotContainer.h"

void UPlanterUI::InitUI(UInventoryComponent* PlayerComponent, UInventoryComponent* PlanterComponent)
{
	PlayerInventoryComponent = PlayerComponent;
	PlanterInventoryComponent = PlanterComponent;

	PlayerInventory->Setup(*PlayerInventoryComponent);
	PlanterInventory->Setup(*PlanterInventoryComponent);
}

void UPlanterUI::UpdateUI()
{
	PlayerInventory->UpdateAllSlots();
	PlanterInventory->UpdateAllSlots();
}

