// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ThoughtBubble/ThoughtBubble.h"
#include "Source/GameInstance/CustomGameInstance.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"

void UThoughtBubble::UpdateText(FDataTableRowHandle ThoughtBubbleEntry, class UCustomGameInstance* GameInstance)
{
	TBData = ThoughtBubbleEntry.GetRow<FThoughtBubbleData>("");

	if (TBData)
	{
		GameInstance->SetTBCooldown(TBData->TimeToDisplay);
		GetWorld()->GetTimerManager().SetTimer(ThoughtBubbleTimerHandle, this, &UThoughtBubble::DestroySelf, TBData->TimeToDisplay, false);

		FullDisplayText = TBData->TextToDisplay.ToString();

		if (!TBData->TextStyle.IsNull())
		{
			FullDisplayText = "<" + TBData->TextStyle.RowName.ToString() + ">" + TBData->TextToDisplay.ToString() + "</>";
		}

		if (TBData->UseTypeWriterEffect)
		{
			GetWorld()->GetTimerManager().SetTimer(ThoughtBubbleTimerHandle, this, &UThoughtBubble::UpdateTextWithTypewriter, 0.1f, true);
		}

		else 
		{
			TXT_DisplayText->SetText(FText::AsCultureInvariant(FullDisplayText));
		}

		AddToViewport();
	}
}

void UThoughtBubble::DestroySelf()
{
	RemoveFromParent();
}

void UThoughtBubble::UpdateTextWithTypewriter()
{

	FullDisplayText = "<" + TBData->TextStyle.RowName.ToString() + ">" + TBData->TextToDisplay.ToString() + "</>";
}
