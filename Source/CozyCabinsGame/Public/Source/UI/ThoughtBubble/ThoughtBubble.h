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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText TextToDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeToDisplay;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool UseTypeWriterEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FDataTableRowHandle TextStyle;

};

UCLASS()
class COZYCABINSGAME_API UThoughtBubble : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle ThoughtBubbleTimerHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> TXT_DisplayText;

	UFUNCTION()
	void UpdateText(FDataTableRowHandle ThoughtBubbleEntry, class UCustomGameInstance* GameInstance);

	UFUNCTION()
	void UpdateTextWithTypewriter();

	UFUNCTION()
	void DestroySelf();

private:

	FString FullDisplayText;
	struct FThoughtBubbleData* TBData;

};
