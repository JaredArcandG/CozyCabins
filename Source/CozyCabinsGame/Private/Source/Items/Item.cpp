// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Items/Item.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/StaticMeshComponent.h"
#include "Source/Constants/TraceChannel.h"
#include "Engine/DataTable.h"
#include <Kismet/GameplayStatics.h>
#include "Source/GameMode/CustomGameModeBase.h"
#include "TimerManager.h"
#include <Source/Player/PlayerCharacter.h>
#include "Source/Components/PlayerInventoryComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	CHECK(Mesh);

	// Set collision response to Interactive Channel (Trace Channel One)
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToChannel(ECC_InteractableChannel, ECollisionResponse::ECR_Block);

	// Assume quantity 1 by default
	ItemDataRowName = FName();
	Quantity = 1;

	// By default, if TTL is -1, the item does not disappear
	TimeToLiveSeconds = -1;

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Get the table pointer from the game mode
	TObjectPtr<ACustomGameModeBase> pGameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	CHECK(pGameMode);

	DataTable = pGameMode->ItemsDataTable;
	CHECK(DataTable);
	CacheItemData(*DataTable);

	// If an explicit TTL was specified in the BP spawn params, then the item should be destroyed by the end of the TTL
	if (TimeToLiveSeconds != -1)
	{
		GetWorld()->GetTimerManager().SetTimer(ItemTimerHandle, this, &AItem::OnDestroy, TimeToLiveSeconds, false, -1.f);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// Gets the cached item data
/// Returns the default item ctor values if invalid
/// </summary>
/// <returns></returns>
FItemData AItem::GetData()
{
	return CachedItemData;
}

/// <summary>
/// Sets the item data
/// </summary>
/// <param name="InMesh"></param>
/// <param name="InDataTableRef"></param>
/// <param name="InItemDataRowName"></param>
/// <param name="InQuantity"></param>
void AItem::SetData(FName InItemDataRowName, int InQuantity)
{
	Quantity = InQuantity;
	ItemDataRowName = InItemDataRowName;
	TimeToLiveSeconds = -1;

	// Clear any past timers on the handle
	GetWorld()->GetTimerManager().ClearTimer(ItemTimerHandle);

	CHECK(DataTable);
	CacheItemData(*DataTable);
}

/// <summary>
/// Sets the item data with TTL
/// </summary>
/// <param name="InItemDataRowName"></param>
/// <param name="InQuantity"></param>
/// <param name="InTimeToLiveSeconds"></param>
void AItem::SetDataWithTTL(FName InItemDataRowName, int InQuantity, int InTimeToLiveSeconds)
{
	SetData(InItemDataRowName, InQuantity);

	TimeToLiveSeconds = InTimeToLiveSeconds;

	// The item should destroy within the TTL specified
	GetWorld()->GetTimerManager().SetTimer(ItemTimerHandle, this, &AItem::OnDestroy, TimeToLiveSeconds, false, -1.f);
}

/// <summary>
/// Caches data from the table so it can be easily retrieved from the actor itself
/// </summary>
void AItem::CacheItemData(const UDataTable& Table)
{
	// Update the cached data
	FString ContextString;
	auto pData = Table.FindRow<FItemData>(ItemDataRowName, ContextString);

	if (pData)
	{
		CachedItemData = *pData;

		// Log the item data for debugging
		UE_LOG(LogTemp, Warning, TEXT("Item Initialized: %s, Name: %s, Description: %s"),
			*ItemDataRowName.ToString(), *CachedItemData.Name, *CachedItemData.Description);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find item data for row: %s"), *ItemDataRowName.ToString());
	}
}

/// <summary>
/// Wrapper to destroy item
/// </summary>
void AItem::OnDestroy()
{
	this->Destroy();
}

/// <summary>
///  Common interact functionality
/// </summary>
/// <param name="World"></param>
/// <param name="SourceCharacter"></param>
void AItem::OnInteract_Implementation(UWorld* World, ACharacter* SourceCharacter)
{
	CHECK(World);
	CHECK(SourceCharacter);

	// Get inventory component from character
	// Try adding to inventory
	auto pPlayer = Cast<APlayerCharacter>(SourceCharacter);
	CHECK(pPlayer);

	auto pInventory = pPlayer->GetInventoryComponent();
	CHECK(pInventory);

	if (pInventory->TryAdd(CachedItemData.Id, Quantity))
	{
		// If successful, notify the parent item spawner of the pickup call
		OnItemInteract.Broadcast();

		// Destroy the item
		this->OnDestroy();
	}

}