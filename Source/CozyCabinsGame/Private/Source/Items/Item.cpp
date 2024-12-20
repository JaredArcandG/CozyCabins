// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Items/Item.h"
#include <Source/Utils/CheckUtils.h>
#include "Components/StaticMeshComponent.h"
#include "Source/Constants/TraceChannel.h"
#include "Engine/DataTable.h"

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
	Quantity = 1;

	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	CHECK(DataTable);

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
