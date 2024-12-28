// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Items/Item.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/StaticMeshComponent.h"
#include "Source/Constants/TraceChannel.h"
#include "Engine/DataTable.h"
#include <Kismet/GameplayStatics.h>
#include "Source/GameMode/CustomGameModeBase.h"

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

	// Assume only one dropped by default
	ItemDataRowName = FName();
	Quantity = 1;

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	CacheItemData();
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

	CacheItemData();
}

/// <summary>
/// Caches data from the table so it can be easily retrieved from the actor itself
/// </summary>
void AItem::CacheItemData()
{
	// Get the table pointer from the game mode
	TObjectPtr<ACustomGameModeBase> pGameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	CHECK(pGameMode);

	DataTable = pGameMode->ItemsDataTable;

	CHECK(DataTable);

	// Update the cached data
	FString ContextString;
	auto pData = DataTable->FindRow<FItemData>(ItemDataRowName, ContextString);

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
