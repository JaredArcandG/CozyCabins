// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Crafting/WorkStationBase.h"
#include "Components/BoxComponent.h"
#include "Source/UI/Crafting/WorkStationUIBase.h"
#include "Engine/DataTable.h"

// Sets default values
AWorkStationBase::AWorkStationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Workstation Mesh"));

}

void AWorkStationBase::DisplayWidget(UWorld* World, ACharacter* SourceCharacter)
{
	if (!IsValid(StationWidget)) 
	{
		if (StationWidgetClass) 
		{
			StationWidget = CreateWidget<UWorkStationUIBase>(World, StationWidgetClass);
		}
	}

	if (!StationWidget->IsInViewport() && IsValid(StationWidget))
	{
		StationWidget->InitializeStation(DataTable, SourceCharacter, FuelItem, RequireFuelItem);
	}
}

// Called when the game starts or when spawned
void AWorkStationBase::BeginPlay()
{
	Super::BeginPlay();
	
}
