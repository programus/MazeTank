// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameWidget.h"
#include "Components/TextBlock.h"

void UEndGameWidget::SetTitle(FText Value)
{
	Title->SetText(Value);
}

void UEndGameWidget::SetLevelDescription(FText Value)
{
	LevelDescription->SetText(Value);
}

void UEndGameWidget::SetCountDown(int32 Seconds)
{
	CountDown->SetText(FText::Format(FTextFormat::FromString("{0}"), Seconds));
}

void UEndGameWidget::SetScoreValue(float Score)
{
	ScoreValue->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt(Score * 10))));
}
