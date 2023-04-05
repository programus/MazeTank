// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TOONTANK_API UEndGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTitle(FText Value);
	void SetLevelDescription(FText Value);
	void SetCountDown(int32 Seconds);
	
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Title;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* LevelDescription;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* CountDown;
};
