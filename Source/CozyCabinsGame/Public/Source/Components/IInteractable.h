// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "IInteractable.generated.h"


 // This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class COZYCABINSGAME_API IInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (Text = "Called when an interaction is detected"))
	void OnInteract(UWorld* World, ACharacter* SourceCharacter);
	virtual void OnInteract_Implementation(UWorld* World, ACharacter* SourceCharacter) {}
};
