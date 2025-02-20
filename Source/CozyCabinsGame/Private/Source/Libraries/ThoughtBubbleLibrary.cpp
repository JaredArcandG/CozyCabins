// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/Libraries/ThoughtBubbleLibrary.h"
#include "Source/GameInstance/CustomGameInstance.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Source/UI/ThoughtBubble/ThoughtBubble.h"

bool UThoughtBubbleLibrary::DisplayThoughtBubble(const UObject* WorldContext, FDataTableRowHandle ThoughtBubbleEntry, TSubclassOf<UThoughtBubble> ThoughtBubbleClass)
{
	if (WorldContext)
	{
		TObjectPtr<UCustomGameInstance> CustomGameInstance = Cast<UCustomGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));

		if (CustomGameInstance)
		{
			if (!CustomGameInstance->ReturnTBValid()) 
			{
				if (!ThoughtBubbleEntry.IsNull()) 
				{
					TObjectPtr<UThoughtBubble> TB = CreateWidget<UThoughtBubble>(WorldContext->GetWorld(), ThoughtBubbleClass);
					TB->UpdateText(ThoughtBubbleEntry, CustomGameInstance);

					return true;
				}
			}
		}
	}

	return false;
}
