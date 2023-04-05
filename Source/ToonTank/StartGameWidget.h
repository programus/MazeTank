// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartGameWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class TOONTANK_API UStartGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTitle(FText Value);
	
protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UTextBlock* Title;
};
