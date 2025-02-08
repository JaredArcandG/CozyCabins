// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Utils/CommonUtils.h"
#include "Kismet/GameplayStatics.h"

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
