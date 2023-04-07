// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTankGameModeBase.generated.h"

class ABasePawn;
class ATower;
class AWall;
class UUserWidget;
class ATankPlayerController;
class UStartGameWidget;
class UEndGameWidget;
struct FTimerHandle;

/**
 * 
 */
UCLASS()
class TOONTANK_API AToonTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void KillActor(AActor* Actor);

protected:
	virtual void BeginPlay() override;
	
private:
	ABasePawn* PlayerPawn;

	UPROPERTY(EditAnywhere, Category = "Game Settings")
		TArray<FText> StartTexts;
	UPROPERTY(EditAnywhere, Category = "Game Settings")
		FText WinTexts[2] = { FText::FromString("You Won!"), FText::FromString("Start next level in")};
	UPROPERTY(EditAnywhere, Category = "Game Settings")
		FText LoseTexts[2] = { FText::FromString("You Lost!"), FText::FromString("Restart in") };
	UPROPERTY(EditAnywhere, Category = "Game Settings")
		float RestartWaitTime = 2;
	UPROPERTY(EditAnywhere, Category = "Game Settings")
		TSubclassOf<UUserWidget> StartGameWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Game Settings")
		TSubclassOf<UUserWidget> EndGameWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Arena")
		TSubclassOf<AWall> EmptyArea;
	UPROPERTY(EditAnywhere, Category = "Arena")
		TSubclassOf<AWall> EdgeWall;
	UPROPERTY(EditAnywhere, Category = "Arena")
		TSubclassOf<AWall> CornerWall;
	UPROPERTY(EditAnywhere, Category = "Arena")
		float BlockSize = 900;
	UPROPERTY(EditAnywhere, Category = "Arena")
		FIntVector2 ArenaSize = FIntVector2(3, 2);
	UPROPERTY(EditAnywhere, Category = "Arena")
		FIntVector2 ArenaSizeIncrease = FIntVector2(2, 1);
	UPROPERTY(EditAnywhere, Category = "Arena")
		float EnemyRate = 0.5;
	UPROPERTY(EditAnywhere, Category = "Arena")
		TSubclassOf<ATower> EnemyClass;

	UPROPERTY(VisibleAnywhere)
		int32 CurrentLevel = 0;

	FTimerHandle TimerHandle;
	UPROPERTY()
	UStartGameWidget* StartGameWidget;
	UPROPERTY()
	UEndGameWidget* EndGameWidget;

	int32 EnemyCount;

	void StartGame();
	void StartGameCountDown(ATankPlayerController* Controller, UStartGameWidget* Widget);

	void EndGame(bool bWin);
	void EndGameCountDown(UEndGameWidget* Widget);

	void BuildArena();
};
