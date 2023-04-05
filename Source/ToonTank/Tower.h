// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class TOONTANK_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	ATower();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Destruct() override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponRecoverTime = 0.5;
	UPROPERTY(EditAnywhere, Category = "Combat")
		int32 MaxProjectileNum = 5;


	void RecoverWeapon();
	void SetWeaponReady(bool bReady);

	ATank* Target;
	bool bWeaponReady;
};
