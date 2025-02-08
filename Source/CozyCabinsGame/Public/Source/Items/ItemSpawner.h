// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Source/SaveGame/UniqueActor.h"
#include "Misc/DateTime.h"
#include <Source/Time/GameTimeManager.h>
#include "ItemSpawner.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct COZYCABINSGAME_API FItemSpawnSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<class AItem> ItemClassToSpawn;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bIsRespawnable = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, meta = (ClampMin = 1.0f, Text="Time in game time that the item should respawn. Must be 1 minute or more"))
	FCustomTimespan RespawnTimeInGameTimeCustom = FCustomTimespan();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int Quantity = 1;

};

/// <summary>
/// Responsible as a wrapper class that dictates spawning logic for items
/// </summary>
UCLASS()
class COZYCABINSGAME_API AItemSpawner : public AUniqueActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AItemSpawner();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Item Settings")
	FItemSpawnSettings SpawnSettings;

	// This is a temporary mesh to see the item in the editor
	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> VisualEditorMesh;

	struct FDateTime PickupTime;

	bool bAwaitingRespawnAfterPlayerPickup;

	FTimespan RespawnTimeInGameTime;

	// Used to track if the item was created in editor or manually on drop
	// Override when created on drop
	bool bOverrideItemDataRespawnSettings;

protected:

	UPROPERTY()
	TObjectPtr<class AItem> SpawnedItem;

	UPROPERTY()
	TObjectPtr<class UGameTimeManager> GameTimeManagerRef;

	UPROPERTY()
	TObjectPtr<class UWorld> WorldRef;

	FTimerHandle SpawnTimeCheckHandle;

	// Time to wait and then retry a spawn if it failed
	static constexpr float TryRespawnRealSeconds = 60.f;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnPreviewMesh();

	bool CheckSpawnItemBeginPlay();

	bool TrySpawnItem();

	UFUNCTION()
	void OnCheckRespawnItemAfterPickup(FTimespan TimePassed, FDateTime CurrentDateTime);

	UFUNCTION()
	void OnCheckSpawnItem();

public:

	void Setup(const TSubclassOf<AItem>& ItemClass, const int& InQuantity);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPickUp();

#if WITH_EDITOR 
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; 
#endif

};
