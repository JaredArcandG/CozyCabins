// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonUtils.generated.h"

/**
 * 
 */
UCLASS()
class COZYCABINSGAME_API UCommonUtils : public UObject
{
	GENERATED_BODY()

public:

	template <typename T>
	static TArray<TObjectPtr<T>> GetAllObjectsInWorld();

	template <typename T>
	static TArray<TObjectPtr<T>> GetAllActorsInWorld(const UWorld& World);
	
};
