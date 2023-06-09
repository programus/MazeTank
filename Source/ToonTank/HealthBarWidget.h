// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class TOONTANK_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealth(float Health, float MaxHealth);

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UProgressBar* HealthBar;
	
};
