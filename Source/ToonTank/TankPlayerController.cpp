// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

void ATankPlayerController::SetPlayerStateEnabled(bool bState)
{
	APawn* OwnerPawn = GetPawn();
	if (bState)
	{
		OwnerPawn->EnableInput(this);
	}
	else
	{
		OwnerPawn->DisableInput(this);
	}
	//bShowMouseCursor = bState;

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(OwnerPawn, &APawn::SetActorTickEnabled, bState);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}
