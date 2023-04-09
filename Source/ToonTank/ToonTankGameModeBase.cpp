// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToonTankGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Wall.h"
#include "BasePawn.h"
#include "Tower.h"
#include "GoalTrigger.h"
#include "TankPlayerController.h"
#include "StartGameWidget.h"
#include "GameRunningWidget.h"
#include "EndGameWidget.h"
#include "MazeGenerator.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#define LEVEL_KEY "Level"
#define SCORE_KEY "Score"

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
			Score += KillScore;
			GameRunningWidget->SetScoreValue(Score);
		}
	}
}

void AToonTankGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	UWorld* World = GetWorld();
	StartGameWidget = Cast<UStartGameWidget>(CreateWidget(World, StartGameWidgetClass));
	GameRunningWidget = Cast<UGameRunningWidget>(CreateWidget(World, GameRunningWidgetClass));
	EndGameWidget = Cast<UEndGameWidget>(CreateWidget(World, EndGameWidgetClass));

	FString LevelOption = UGameplayStatics::ParseOption(OptionsString, TEXT(LEVEL_KEY));
	FString ScoreOption = UGameplayStatics::ParseOption(OptionsString, TEXT(SCORE_KEY));
	CurrentLevel = FCString::Atoi(*LevelOption);
	Score = FCString::Atof(*ScoreOption);

	UE_LOG(LogTemp, Warning, TEXT("Options: %s, Score S: %s, Score: %f"), *OptionsString, *ScoreOption, Score);

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
		GameRunningWidget->SetScoreValue(Score);
		GameRunningWidget->AddToViewport();
	}
}

void AToonTankGameModeBase::EndGame(bool bWin)
{
	if (bGameRunning)
	{
		FText* Texts = bWin ? WinTexts : LoseTexts;
		EndGameWidget->SetScoreValue(Score);
		if (bWin)
		{
			CurrentLevel++;
			Score += CurrentLevel * LevelScoreFactor;
			GameRunningWidget->SetScoreValue(Score);
			EndGameWidget->SetScoreValue(Score);
		}
		else
		{
			Score = 0;
		}
		EndGameWidget->SetTitle(Texts[0]);
		EndGameWidget->SetLevelDescription(Texts[1]);
		EndGameWidget->AddToViewport();
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AToonTankGameModeBase::EndGameCountDown, EndGameWidget);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, true);
		bGameRunning = false;
	}
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

		FString Options = FString::Printf(TEXT("%s=%d?%s=%f"), TEXT(LEVEL_KEY), CurrentLevel, TEXT(SCORE_KEY), Score);
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

			if (!(r == 0 && c == 0) && FMath::RandRange(0, 1) < EnemyRate)
			{
				World->SpawnActor<ATower>(EnemyClass, Location, FRotator::ZeroRotator);
				EnemyCount++;
			}
		}
		Location.X += BlockSize;
	}
	Location.X -= BlockSize;

	if (EnemyCount <= 0)
	{
		World->SpawnActor<ATower>(EnemyClass, Location, FRotator::ZeroRotator);
		EnemyCount++;
	}
	World->SpawnActor<AGoalTrigger>(GoalTriggerClass, Location, FRotator::ZeroRotator);

	FString MazeString(TEXT("Maze:\n\n"));
	MazeString.Append(MazeGenerator.MazeString());

	UE_LOG(LogTemp, Warning, TEXT("%s"), *MazeString);
}
