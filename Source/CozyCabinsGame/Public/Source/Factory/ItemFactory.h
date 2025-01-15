// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemFactory.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UItemFactory : public UObject
{
	GENERATED_BODY()

public:

	static TObjectPtr<class AItem> SpawnItemInWorld(UWorld& World, const FTransform& Transform, TSubclassOf<class AItem> ItemClass, const FGuid& ItemId, const int& Quantity, const bool& AddTTL=true);

	static TObjectPtr<class AItem> SpawnBPDefaultItemInWorld(UWorld& World, const FTransform& Transform, TSubclassOf<class AItem> ItemClass, const int& Quantity);
	
};
