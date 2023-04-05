// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTankGameModeBase.generated.h"

class ABasePawn;
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
};
