// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniqueActor.generated.h"

UCLASS()
class COZYCABINSGAME_API AUniqueActor : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AUniqueActor();

	UPROPERTY(VisibleAnywhere, Category = "Id")
	FGuid CustomUniqueId;

	FORCEINLINE FGuid GetCustomUniqueId() { return CustomUniqueId; }

};
