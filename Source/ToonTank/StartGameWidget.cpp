// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameWidget.h"
#include "Components/TextBlock.h"

void UStartGameWidget::SetTitle(FText Value)
{
	Title->SetText(Value);
}
