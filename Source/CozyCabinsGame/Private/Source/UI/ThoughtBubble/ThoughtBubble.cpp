// Fill out your copyright notice in the Description page of Project Settings.


#include "Source/UI/ThoughtBubble/ThoughtBubble.h"
#include "Source/GameInstance/CustomGameInstance.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"

void UThoughtBubble::UpdateText(FDataTableRowHandle ThoughtBubbleEntry, class UCustomGameInstance* GameInstance)
{
	CustomGameInstance = GameInstance;
	TBData = ThoughtBubbleEntry.GetRow<FThoughtBubbleData>("");

	CustomGameInstance->SetTBWidget(this);
	float TimerTime = TBData->TimeToDisplay;
	GetWorld()->GetTimerManager().SetTimer(ThoughtBubbleTimerHandle, this, &UThoughtBubble::DestroySelf, TimerTime, false);


	if (TBData)
	{
		FString DisplayText = TBData->TextToDisplay.ToString();

		if (!TBData->TextStyle.IsNull())
		{
			DisplayText = "<" + TBData->TextStyle.RowName.ToString() + ">" + TBData->TextToDisplay.ToString() + "</>";
		}

		if (TBData->UseTypeWriterEffect)
		{
			TypeWriterIndex = 0;
			CurrentText = "";
			GetWorld()->GetTimerManager().SetTimer(ThoughtBubbleTypewriterHandle, this, &UThoughtBubble::UpdateTextWithTypewriter, TypewriterSpeed, true);
		}
		else 
		{
			TXT_DisplayText->SetText(FText::AsCultureInvariant(DisplayText));
			Invalidate(EInvalidateWidget::LayoutAndVolatility | EInvalidateWidgetReason::Prepass);
		}

	}

	AddToViewport();
}

void UThoughtBubble::DestroySelf()
{
	CustomGameInstance->SetTBWidget(nullptr);
	RemoveFromParent();
}

void UThoughtBubble::UpdateTextWithTypewriter()
{
	if (CurrentText == TBData->TextToDisplay.ToString())
	{
		GetWorld()->GetTimerManager().ClearTimer(ThoughtBubbleTypewriterHandle);
	}
	else 
	{
		CurrentText += TBData->TextToDisplay.ToString()[TypeWriterIndex];
		FString DisplayText = CurrentText;

		if (!TBData->TextStyle.IsNull()) 
		{
			DisplayText = "<" + TBData->TextStyle.RowName.ToString() + ">" + CurrentText + "</>";
		}

		TXT_DisplayText->SetText(FText::AsCultureInvariant(DisplayText));
		Invalidate(EInvalidateWidget::LayoutAndVolatility | EInvalidateWidgetReason::Prepass);

		TypeWriterIndex += 1;
	}
}
