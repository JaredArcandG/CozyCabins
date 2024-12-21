// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Source/Crafting/CraftingData.h"
#include "CraftingRecipeSlot.generated.h"


UCLASS()
class COZYCABINSGAME_API UCraftingRecipeSlot : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ButtonSelect;


public:

	void InitializeSlot(FCraftingRecipe* Recipe);
};
