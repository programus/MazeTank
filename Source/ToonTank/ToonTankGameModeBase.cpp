// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToonTankGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Wall.h"
#include "BasePawn.h"
#include "Tower.h"
#include "TankPlayerController.h"
#include "StartGameWidget.h"
#include "EndGameWidget.h"
#include "MazeGenerator.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#define LEVEL_KEY "Level"

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

	FString LevelOption = UGameplayStatics::ParseOption(OptionsString, TEXT(LEVEL_KEY));
	CurrentLevel = FCString::Atoi(*LevelOption);

	BuildArena();

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
	if (bWin)
	{
		CurrentLevel++;
	}
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

		FString Options = FString::Printf(TEXT("%s=%d"), TEXT(LEVEL_KEY), CurrentLevel);
		UGameplayStatics::OpenLevel(this, FName("/Game/Maps/Main"), true, Options);
	}
}

void AToonTankGameModeBase::BuildArena()
{
	EnemyCount = 0;
	UWorld* World = GetWorld();
	uint32 w = ArenaSize.X + ArenaSizeIncrease.X * CurrentLevel;
	uint32 h = ArenaSize.Y + ArenaSizeIncrease.Y * CurrentLevel;
	CMazeGenerator MazeGenerator(w, h);
	const uint32** Maze = MazeGenerator.Generate();

	FVector Location(-BlockSize, -BlockSize, 0);
	FRotator VerticleRotation(0, 90, 0);
	World->SpawnActor<AWall>(CornerWall, Location, FRotator::ZeroRotator);
	for (uint32 i = 0; i < w; i++)
	{
		Location.Y += BlockSize;
		World->SpawnActor<AWall>(EdgeWall, Location, FRotator::ZeroRotator);
		World->SpawnActor<AWall>(CornerWall, Location, FRotator::ZeroRotator);
	}
	Location.X += BlockSize;
	for (uint32 r = 0; r < h; r++)
	{
		Location.Y = -BlockSize;
		World->SpawnActor<AWall>(EdgeWall, Location, VerticleRotation);
		World->SpawnActor<AWall>(CornerWall, Location, FRotator::ZeroRotator);
		for (uint32 c = 0; c < w; c++)
		{
			Location.Y += BlockSize;
			World->SpawnActor<AWall>(EmptyArea, Location, FRotator::ZeroRotator);
			AWall* Top = World->SpawnActor<AWall>(EdgeWall, Location, FRotator::ZeroRotator);
			AWall* Right = World->SpawnActor<AWall>(EdgeWall, Location, VerticleRotation);
			AWall* Corner = World->SpawnActor<AWall>(CornerWall, Location, FRotator::ZeroRotator);
			bool bVPath = Maze[r][c] & MAZE_V_PATH;
			bool bHPath = Maze[r][c] & MAZE_H_PATH;
			if (bVPath)
			{
				Top->HideWall();
			}
			if (bHPath)
			{
				Right->HideWall();
			}
			if (bVPath && bHPath)
			{
				Corner->HideWall();
			}

			if (!(r == 0 && c == 0) && FMath::RandRange(0, 1) < EnemyRate)
			{
				World->SpawnActor<ATower>(EnemyClass, Location, FRotator::ZeroRotator);
				EnemyCount++;
			}
		}
		Location.X += BlockSize;
	}

	if (EnemyCount <= 0)
	{
		Location.X -= BlockSize;
		World->SpawnActor<ATower>(EnemyClass, Location, FRotator::ZeroRotator);
		EnemyCount++;
	}

	FString MazeString(TEXT("Maze:\n\n"));
	MazeString.Append(MazeGenerator.MazeString());

	UE_LOG(LogTemp, Warning, TEXT("%s"), *MazeString);
}
