// Fill out your copyright notice in the Description page of Project Settings.


#include "GameRunningWidget.h"
#include "Components/TextBlock.h"

void UGameRunningWidget::SetScoreValue(float Score)
{
	TargetScore = Score * 10;
}

void UGameRunningWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	CurrentScore = FMath::FInterpTo(
		CurrentScore,
		TargetScore,
		InDeltaTime,
		ScoreChangeSpeed
	);

	FLinearColor Color(ScoreValue->GetColorAndOpacity().GetSpecifiedColor());

	float GreenFactor = FMath::Min((TargetScore - CurrentScore) / 10, 1);
	Color.R = Color.B = Color.G - GreenFactor;

	ScoreValue->SetColorAndOpacity(FSlateColor(Color));

	FString Value(FString::FromInt(FMath::CeilToInt(CurrentScore)));
	ScoreValue->SetText(FText::FromString(Value));
}
