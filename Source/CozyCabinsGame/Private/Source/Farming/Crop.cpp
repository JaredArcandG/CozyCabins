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

	TimePlanted = GameTimeManager->GetCurrentGameTime();
}

void ACrop::UpdateTime()
{
	FTimespan TimePassed = GameTimeManager->GetTimeDifferenceFromCurrentTime(TimePlanted);

	ECropStage NewCropStage = CheckCropStage(TimePassed);

	if (CurrentCropStage != NewCropStage) 
	{
		UpdateCropStage(NewCropStage);
	}
}

ECropStage ACrop::CheckCropStage(FTimespan TimePassed)
{
	float PassedTime = TimePassed.GetTotalMinutes();
	ECropStage NewCropStage = ECropStage::StageOne;

	float AccumulatedTime = 0.0f;

	TArray<ECropStage> Keys;
	ItemData.CropStageData.GetKeys(Keys);

	for (ECropStage CropStageInfo : Keys) 
	{
		float StageDuration = ItemData.CropStageData.FindRef(CropStageInfo).StageTime;

		if (PassedTime >= AccumulatedTime && PassedTime < AccumulatedTime + StageDuration)
		{
			NewCropStage = CropStageInfo;
			break;
		}

		AccumulatedTime += StageDuration;
	}

	if (CurrentCropStage == ECropStage::StageThree && PassedTime >= AccumulatedTime)
	{
		return ECropStage::Completed;
	}

	return NewCropStage;
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


