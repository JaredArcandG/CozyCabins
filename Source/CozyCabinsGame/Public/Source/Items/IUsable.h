// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "IUsable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUsable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for items for logic to use/unequip them 
 */
class COZYCABINSGAME_API IUsable
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUse(UWorld* World, ACharacter* SourceCharacter);
	void OnUse_Implementation(UWorld* World, ACharacter* SourceCharacter) { }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnUnequip(UWorld* World, ACharacter* SourceCharacter);
	void OnUnequip_Implementation(UWorld* World, ACharacter* SourceCharacter) { }

	FORCEINLINE void OnDrop(UWorld* World, ACharacter* SourceCharacter) { }

};
