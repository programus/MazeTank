// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameRunningWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TOONTANK_API UGameRunningWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScoreValue(float Score);

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ScoreValue;
	UPROPERTY(EditAnywhere)
		float ScoreChangeSpeed = 10;

	float TargetScore;
	float CurrentScore;
};
