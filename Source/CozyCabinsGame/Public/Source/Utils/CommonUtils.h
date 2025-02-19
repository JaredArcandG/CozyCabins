// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
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

template<typename T>
TArray<TObjectPtr<T>> UCommonUtils::GetAllObjectsInWorld()
{
	TArray<T*> resultArr;

	for (TObjectIterator<T> iter; iter; iter++)
	{
		if (iter->GetObject())
		{
			resultArr.Add(Cast<T>(iter->GetObject()));
		}
	}

}

template<typename T>
TArray<TObjectPtr<T>> UCommonUtils::GetAllActorsInWorld(const UWorld& World)
{
	TArray<TObjectPtr<T>> resultArr;
	TArray<AActor*> outArr;

	UGameplayStatics::GetAllActorsOfClass(&World, T::StaticClass(), outArr);

	for (auto pActor : outArr)
	{
		resultArr.Add(Cast<T>(pActor));
	}

	return resultArr;
}

