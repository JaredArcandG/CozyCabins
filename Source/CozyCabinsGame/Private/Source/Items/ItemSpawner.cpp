// Fill out your copyright notice in the Description page of Project Settings.

#include "Source/Items/ItemSpawner.h"
#include "Source/Items/Item.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/StaticMeshComponent.h"
#include <Source/Factory/ItemFactory.h>
#include "GameFramework/GameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpawnedItem = nullptr;
	bAwaitingRespawnAfterPlayerPickup = false;

	VisualEditorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Editor Preview Mesh"));

	bOverrideItemDataRespawnSettings = false;

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// No longer need the preview mesh
	CHECK(VisualEditorMesh);
	VisualEditorMesh->SetStaticMesh(nullptr);

	WorldRef = GetWorld();

	TObjectPtr<AGameModeBase> pGameMode = UGameplayStatics::GetGameMode(WorldRef);
	CHECK(pGameMode);

	GameTimeManagerRef = pGameMode->FindComponentByClass<UGameTimeManager>();
	CHECK(GameTimeManagerRef);

	if (!bOverrideItemDataRespawnSettings)
	{
		RespawnTimeInGameTime = SpawnSettings.RespawnTimeInGameTimeCustom.ConvertToFTimespan();
	}

	// If awaiting respawn, try spawn item fails, but we need to set the timer appropriately to try again
	// Otherwise, it spawns the item normally
	if (!CheckSpawnItemBeginPlay())
	{
		CHECK(WorldRef);
		WorldRef->GetTimerManager().SetTimer(SpawnTimeCheckHandle, this, &AItemSpawner::OnCheckSpawnItem, TryRespawnRealSeconds, true, -1.f);
	}

}

/// <summary>
/// Spawns the item preview mesh in the editor
/// Should NOT be used for in game logic
/// </summary>
void AItemSpawner::SpawnPreviewMesh()
{
	CHECK(VisualEditorMesh);
	CHECK(SpawnSettings.ItemClassToSpawn);

	auto pItem = NewObject<AItem>(this, SpawnSettings.ItemClassToSpawn);
	CHECK(pItem);

	FString sDataTablePath = TEXT("/Game/DataTables/ItemsDataTable.ItemsDataTable");
	TObjectPtr<UDataTable> pDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *sDataTablePath));
	CHECK(pDataTable);

	pItem->CacheItemData(*pDataTable);

	FItemData itemData = pItem->GetData();
	VisualEditorMesh->SetStaticMesh(itemData.Mesh);

	pItem->Destroy();
	
}

/// <summary>
/// Set up item spawning logic in begin play (if there is no item for whatever reason, like from a save, then we need to add the respawn callback check again)
/// </summary>
/// <returns></returns>
bool AItemSpawner::CheckSpawnItemBeginPlay()
{

	// TODO: Update after saving/load is implemented
	// Spawner is waiting for the respawn (triggered when a player picks it up), calculate how much game time has passed, create spawner
	if (bAwaitingRespawnAfterPlayerPickup)
	{
		GameTimeManagerRef->OnGameTimePassed.AddUniqueDynamic(this, &AItemSpawner::OnCheckRespawnItemAfterPickup);
		return true;
	}

	return TrySpawnItem();

}

/// <summary>
/// Attempts to spawn the item where it was in its original position
/// </summary>
/// <returns>True if spawn is successful, false otherwise</returns>
bool AItemSpawner::TrySpawnItem()
{
	// Spawner should spawn the item if possible
	if (WorldRef)
	{
		SpawnedItem = UItemFactory::SpawnBPDefaultItemInWorld(*WorldRef, GetActorTransform(), SpawnSettings.ItemClassToSpawn, SpawnSettings.Quantity);

		if (SpawnedItem)
		{
			SpawnedItem->OnItemInteract.AddUniqueDynamic(this, &AItemSpawner::OnPickUp);

			if (bOverrideItemDataRespawnSettings)
			{
				SpawnSettings.bIsRespawnable = SpawnedItem->GetData().IsRespawnableOnDrop;
				RespawnTimeInGameTime = SpawnedItem->GetData().RespawnTimeInGameTime.ConvertToFTimespan();
			}

			AccumulatedTime = FTimespan(0);

			return true;
		}
	}

	return false;
}

/// <summary>
/// Checks if item needs to be respawned after player pickup, then respawns the item
/// </summary>
void AItemSpawner::OnCheckRespawnItemAfterPickup(FTimespan TimePassed, FDateTime CurrentDateTime)
{
	CHECK(GameTimeManagerRef);

	// Only look to respawn if it's possible
	if (bAwaitingRespawnAfterPlayerPickup && !SpawnedItem && SpawnSettings.bIsRespawnable)
	{
		if (AccumulatedTime >= RespawnTimeInGameTime)
		{
			if (TrySpawnItem())
			{
				// Clear check respawn item event
				bAwaitingRespawnAfterPlayerPickup = false;
				GameTimeManagerRef->OnGameTimePassed.RemoveDynamic(this, &AItemSpawner::OnCheckRespawnItemAfterPickup);
			}
		}
		else 
		{
			AccumulatedTime += TimePassed;
		}
	}
}

/// <summary>
/// Wrapper to TrySpawnItem
/// </summary>
void AItemSpawner::OnCheckSpawnItem()
{
	// Not respawnable or already exists
	if (!SpawnSettings.bIsRespawnable || SpawnedItem)
	{
		return;
	}

	if (TrySpawnItem())
	{
		CHECK(WorldRef);

		// Clear respawn timer if exists
		WorldRef->GetTimerManager().ClearTimer(SpawnTimeCheckHandle);

	}
}

/// <summary>
/// Setup to be used with deferred spawn to manually change the variables
/// </summary>
/// <param name="ItemClass"></param>
/// <param name="IsRespawnable"></param>
/// <param name="InRespawnTimeInGameTimeCustom"></param>
void AItemSpawner::Setup(const TSubclassOf<AItem>& ItemClass, const int& InQuantity)
{
	SpawnSettings.ItemClassToSpawn = ItemClass;
	SpawnSettings.Quantity = InQuantity;
	bOverrideItemDataRespawnSettings = true;
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// Start the respawn timer, flag
/// </summary>
void AItemSpawner::OnPickUp()
{
	CHECK(GameTimeManagerRef);

	// If the item is not respawnable, we ne longer need the spawner
	if (!SpawnSettings.bIsRespawnable)
	{
		this->Destroy();
		return;
	}

	bAwaitingRespawnAfterPlayerPickup = true;
	GameTimeManagerRef->OnGameTimePassed.AddUniqueDynamic(this, &AItemSpawner::OnCheckRespawnItemAfterPickup);
	SpawnedItem = nullptr;
	PickupTime = GameTimeManagerRef->GetCurrentGameTime();
}

#if WITH_EDITOR 
void AItemSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SpawnPreviewMesh();
}

void AItemSpawner::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	// Check if the property that changed is the item class
	static const FName spawnSettingsPropertyName = GET_MEMBER_NAME_CHECKED(AItemSpawner, SpawnSettings.ItemClassToSpawn);
	FString sSpawnSettingsNodeName = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetValue()->GetName();
	FString sItemClassNodeName = FString();

	if (auto pNode = PropertyChangedEvent.PropertyChain.GetActiveMemberNode()->GetNextNode())
	{
		sItemClassNodeName = pNode->GetValue()->GetName();
	}

	FName fullPropertyName = FName(*(sSpawnSettingsNodeName + FString(".") + sItemClassNodeName));

	if (fullPropertyName == spawnSettingsPropertyName)
	{
		// Update the visual editor mesh
		SpawnPreviewMesh();
	}
}
#endif
