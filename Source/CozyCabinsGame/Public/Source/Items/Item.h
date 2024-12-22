// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IUsable.h"
#include "Source/Components/IInteractable.h"
#include "ItemData.h"
#include "Item.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API AItem : public AActor, public IUsable, public IInteractable
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UDataTable> DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemDataRowName;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int Quantity;

protected:

	FItemData CachedItemData;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the item specific data like name, description, etc."))
	FItemData GetData();

};
