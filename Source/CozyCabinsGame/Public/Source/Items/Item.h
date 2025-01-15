// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IUsable.h"
#include "Source/Components/IInteractable.h"
#include "ItemData.h"
#include "Engine/TimerHandle.h"
#include "Item.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COZYCABINSGAME_API AItem : public AActor, public IUsable, public IInteractable
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemInteract);
	
	FOnItemInteract OnItemInteract;

	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemDataRowName;

	UPROPERTY(BlueprintReadWrite)
	int Quantity;

protected:

	FItemData CachedItemData;

	UPROPERTY()
	TObjectPtr<class UDataTable> DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeToLiveSeconds;

	FTimerHandle ItemTimerHandle;


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (Text = "Get the item specific data like name, description, etc."))
	FItemData GetData();

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the item specific data mesh, data table, row name, etc."))
	void SetData(FName InItemDataRowName, int InQuantity);

	UFUNCTION(BlueprintCallable, meta = (Text = "Set the item specific data mesh, data table, row name, etc. with a time to live. Item disappears after TTL expires"))
	void SetDataWithTTL(FName InItemDataRowName, int InQuantity, int InTimeToLiveSeconds);

	virtual void OnInteract_Implementation(UWorld* World, ACharacter* SourceCharacter) override;

private:

	void CacheItemData();

	void OnDestroy();


};
