// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Farming/Planter.h"
#include "Source/Farming/Crop.h"
#include "Source/Components/InventoryComponent.h"
#include "Source/Time/GameTimeManager.h"
#include "Source/Items/ItemData.h"
#include "Source/GameMode/CustomGameModeBase.h"
#include "Source/Items/Item.h"
#include "Source/UI/PlanterUI.h"
#include "Source/Player/PlayerCharacter.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlanter::APlanter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PlanterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planter Mesh"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

// Called when the game starts or when spawned
void APlanter::BeginPlay()
{
	Super::BeginPlay();

	// Fill Farmin Spots by getting all SceneComponents attached to the Mesh
	TArray<TObjectPtr<USceneComponent>> FarmSpots = PlanterMesh->GetAttachChildren();

	for (TObjectPtr<USceneComponent> FarmSpot : FarmSpots)
	{
		FarmingSpots.Add(FarmSpot, nullptr);
	}

	DebugTest();

	ACustomGameModeBase* GameMode = Cast<ACustomGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GameTimeManager = GameMode->GetComponentByClass<UGameTimeManager>();
}


void APlanter::DebugTest()
{
	for (FDataTableRowHandle Item : DebugItems) 
	{
		FItemData* Data = Item.DataTable->FindRow<FItemData>(Item.RowName, "");
		InventoryComponent->TryAdd(Data->Id, 3);
	}
}

void APlanter::DisplayWidget(UWorld* World, ACharacter* SourceCharacter)
{
	if (!IsValid(PlanterUIWidget))
	{
		if (PlanterUIWidgetClass)
		{
			PlanterUIWidget = CreateWidget<UPlanterUI>(World, PlanterUIWidgetClass);
			APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			UInventoryComponent* PlayerInventory = Player->FindComponentByClass<UInventoryComponent>();

			PlanterUIWidget->InitUI(PlayerInventory, InventoryComponent);
		}
	}

	if (!PlanterUIWidget->IsInViewport() && IsValid(PlanterUIWidget))
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;

		PlanterUIWidget->UpdateUI();
		PlanterUIWidget->AddToViewport(10);
	}
}

void APlanter::TimePassed(FTimespan PassedTime)
{
	UpdateTimeOfCrops(PassedTime);
	CheckAndFillEmptySlots();
}

void APlanter::UpdateTimeOfCrops(FTimespan PassedTime)
{
	TArray<TObjectPtr<USceneComponent>> Keys;
	FarmingSpots.GetKeys(Keys);

	for (TObjectPtr<USceneComponent> Key : Keys) 
	{
		if (FarmingSpots.FindRef(Key)) 
		{
			if (TObjectPtr<ACrop> Crop = Cast<ACrop>(FarmingSpots.FindRef(Key)))
			{
				Crop->UpdateTime(PassedTime);
			}
		}
	}
}

void APlanter::PlantSeed(USceneComponent* FarmSpot, FItemData ItemData)
{
	if (FarmSpot) 
	{
		FVector Location = FarmSpot->GetComponentLocation();
		FRotator Rotation(0.0f, 0.0f, 0.0f);

		TObjectPtr<ACrop> NewCrop = GetWorld()->SpawnActor<ACrop>(CropClass, Location, Rotation);
		FarmingSpots.Add(FarmSpot, NewCrop);

		NewCrop->InitCrop(ItemData, this, GameTimeManager, FarmSpot);

		InventoryComponent->TryRemove(ItemData.Id, 1);
	}
}

void APlanter::CheckAndFillEmptySlots()
{
	TArray<TObjectPtr<USceneComponent>> Keys;
	FarmingSpots.GetKeys(Keys);

	for (TObjectPtr<USceneComponent> Key : Keys)
	{
		if (!FarmingSpots.FindRef(Key))
		{
			FItemData ItemData;
			int Quantity;

			FItemSearchFilterParams Params;
			Params.ItemCategory = EItemCategory::Plantable;

			TArray<int> Indexes = InventoryComponent->GetIndexesWithItemFilter(Params);
			for (int Index : Indexes) 
			{
				if (InventoryComponent->TryGetItemAtIndex(Index, ItemData, Quantity))
				{
					if (ItemData.ItemCategories.Contains(EItemCategory::Plantable))
					{
						PlantSeed(Key, ItemData);
						break;
					}
				}
			}
		}
	}
}

void APlanter::CropFinished(USceneComponent* FarmSpot, FItemData ItemData, AActor* CropActor)
{
	FTransform TempTransform(FRotator(0.0f, 0.0f, 0.0f), FarmSpot->GetComponentLocation(), FVector(1.0f));

	TObjectPtr<AItem> NewHarvestable = GetWorld()->SpawnActor<AItem>(ItemData.ItemClass, TempTransform);

	FarmingSpots.Add(FarmSpot, NewHarvestable);

	CropActor->Destroy();
}
