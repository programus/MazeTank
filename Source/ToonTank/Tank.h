// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

class UInputControlComponent;

/**
 * 
 */
UCLASS()
class TOONTANK_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual void Destruct() override;
	void SetAutoAim(bool bEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;

	UPROPERTY(EditAnywhere, Category = "Aim")
		float AutoAimSensorRadius = 500;
	UPROPERTY(EditAnywhere, Category = "Aim")
		float AutoAimRange = 5000;
private:
	UInputControlComponent* InputControlComp;
	UInputControlComponent* GetInputControlComp();

	bool bAutoAim;

	bool bOriginalRotateToTarget;

	void AutoAim();
};
