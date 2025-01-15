// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Farming/Crop.h"
#include "Source/Farming/Planter.h"
#include "Engine/DataTable.h"
#include "Source/Time/GameTimeManager.h"

// Sets default values
ACrop::ACrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Planter Mesh"));
	CropMesh->SetWorldScale3D(FVector(0.25f));
}

// Called when the game starts or when spawned
void ACrop::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrop::InitCrop(FItemData ItemDataInput, APlanter* Planter, UGameTimeManager* TimeManager, USceneComponent* SceneComponent)
{
	ItemData = ItemDataInput;

	UpdateCropStage(ECropStage::StageOne);

	GameTimeManager = TimeManager;
	ParentPlanter = Planter;
	FarmSpot = SceneComponent;

	TimePassedSincePlanted = FTimespan(0, 0, 0, 0, 0);
}

void ACrop::UpdateTime(FTimespan PassedTime)
{
	TimePassedSincePlanted += PassedTime;

	ECropStage NewCropStage = CheckCropStage();

	if (CurrentCropStage != NewCropStage) 
	{
		UpdateCropStage(NewCropStage);
	}
}

ECropStage ACrop::CheckCropStage()
{
	float TimePassed = TimePassedSincePlanted.GetTotalMinutes();

	float AccumulatedTime = 0.0f;

	TArray<ECropStage> Keys;
	ItemData.CropStageData.GetKeys(Keys);

	for (ECropStage CropStageInfo : Keys) 
	{
		AccumulatedTime += ItemData.CropStageData.FindRef(CropStageInfo).StageTime;

		if (TimePassed < AccumulatedTime)
		{
			return CropStageInfo;
		}

	}

	return ECropStage::Completed;
}

void ACrop::UpdateCropStage(ECropStage NewCropStage)
{
	CurrentCropStage = NewCropStage;

	if (CurrentCropStage == ECropStage::Completed) 
	{
		HandleCropFinish();
	}
	else 
	{
		CropMesh->SetStaticMesh(ItemData.CropStageData.Find(CurrentCropStage)->Mesh);
	}
}

void ACrop::HandleCropFinish()
{
	ParentPlanter->CropFinished(FarmSpot, ItemData, this);
}


