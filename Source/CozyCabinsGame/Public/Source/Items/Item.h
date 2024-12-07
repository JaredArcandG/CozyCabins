// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/Image.h>
#include "IUsable.h"
#include "Source/Components/IInteractable.h"
#include "Item.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UImage> Icon;

};

UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API AItem : public AActor, public IUsable, public IInteractable
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
