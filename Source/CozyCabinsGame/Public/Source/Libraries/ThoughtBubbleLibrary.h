// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ThoughtBubbleLibrary.generated.h"

class UThoughtBubble;

UCLASS()
class COZYCABINSGAME_API UThoughtBubbleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static bool DisplayThoughtBubble(const UObject* WorldContext, FDataTableRowHandle ThoughtBubbleEntry, TSubclassOf<UThoughtBubble> ThoughtBubbleClass);
};
