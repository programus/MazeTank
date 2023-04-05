// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToonTankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BasePawn.h"
#include "Tower.h"
#include "TankPlayerController.h"
#include "StartGameWidget.h"
#include "EndGameWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void AToonTankGameModeBase::KillActor(AActor* Actor)
{
	if (ABasePawn* Pawn = Cast<ABasePawn>(Actor))
	{
		Pawn->Destruct();
		if (Pawn == PlayerPawn)
		{
			bool bStillAlive = Pawn->InputEnabled();
			if (ATankPlayerController* TankController = Cast<ATankPlayerController>(PlayerPawn->GetController()))
			{
				TankController->SetPlayerStateEnabled(false);
			}
			if (bStillAlive)
			{
				EndGame(false);
			}
		}
		else
		{
			EnemyCount--;
			if (EnemyCount <= 0)
			{
				EndGame(true);
			}
		}
	}
}

void AToonTankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	StartGameWidget = Cast<UStartGameWidget>(CreateWidget(GetWorld(), StartGameWidgetClass));
	EndGameWidget = Cast<UEndGameWidget>(CreateWidget(GetWorld(), EndGameWidgetClass));

	StartGame();
}

void AToonTankGameModeBase::StartGame()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Enemies);
	EnemyCount = Enemies.Num();
	if (ATankPlayerController* TankController = Cast<ATankPlayerController>(PlayerPawn->GetController()))
	{
		TankController->SetPlayerStateEnabled(false);
		// Setup UI
		StartGameWidget->SetTitle(StartTexts[0]);
		StartGameWidget->AddToViewport();

		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AToonTankGameModeBase::StartGameCountDown, TankController, StartGameWidget);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, true);
	}
}

void AToonTankGameModeBase::StartGameCountDown(ATankPlayerController* Controller, UStartGameWidget* Widget)
{
	static int32 CountIndex = 0;
	int32 Index = CountIndex + 1;

	Widget->SetTitle(StartTexts[Index]);

	CountIndex++;
	CountIndex %= StartTexts.Num() - 1;

	if (CountIndex == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		UWidgetLayoutLibrary::RemoveAllWidgets(this);
		Controller->SetPlayerStateEnabled(true);
	}
}

void AToonTankGameModeBase::EndGame(bool bWin)
{
	FText* Texts = bWin ? WinTexts : LoseTexts;
	EndGameWidget->SetTitle(Texts[0]);
	EndGameWidget->SetLevelDescription(Texts[1]);
	EndGameWidget->AddToViewport();
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AToonTankGameModeBase::EndGameCountDown, EndGameWidget);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, true);
}

void AToonTankGameModeBase::EndGameCountDown(UEndGameWidget* Widget)
{
	static int32 Count = (int32)RestartWaitTime;

	Widget->SetCountDown(Count);

	Count--;
	if (Count < 0)
	{
		Count = (int32)RestartWaitTime;
		UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(TimerHandle);
		UWidgetLayoutLibrary::RemoveAllWidgets(this);

		UGameplayStatics::OpenLevel(this, FName(World->GetMapName()));
	}
}
