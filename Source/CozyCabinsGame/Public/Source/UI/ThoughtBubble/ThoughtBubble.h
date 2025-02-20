// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ThoughtBubble.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct COZYCABINSGAME_API FThoughtBubbleData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TextToDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseTypeWriterEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DisplayOnceOnly = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle TextStyle;

};

UCLASS()
class COZYCABINSGAME_API UThoughtBubble : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TypewriterSpeed = 0.05f;

	UPROPERTY()
	FTimerHandle ThoughtBubbleTimerHandle;

	UPROPERTY()
	FTimerHandle ThoughtBubbleTypewriterHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> TXT_DisplayText;

	UPROPERTY()
	TObjectPtr<class UCustomGameInstance> CustomGameInstance;

	UFUNCTION()
	void UpdateText(FDataTableRowHandle ThoughtBubbleEntry, class UCustomGameInstance* GameInstance);

	UFUNCTION()
	void UpdateTextWithTypewriter();

	UFUNCTION()
	void DestroySelf();

private:

	struct FThoughtBubbleData* TBData;

	UPROPERTY()
	FString CurrentText;

	UPROPERTY()
	int TypeWriterIndex;

};
