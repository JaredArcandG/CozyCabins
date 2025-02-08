// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/SaveGame/UniqueActor.h"

// Sets default values
AUniqueActor::AUniqueActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!CustomUniqueId.IsValid())
	{
		CustomUniqueId = FGuid::NewGuid();
	}

}
