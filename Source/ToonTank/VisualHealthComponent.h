// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "VisualHealthComponent.generated.h"


class UHealthBarWidget;
class AToonTankGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANK_API UVisualHealthComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVisualHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Combat")
		float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
		float CurrentHealth;
		
	UFUNCTION()
	void OnDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	AToonTankGameModeBase* GameMode;
	UHealthBarWidget* HealthBarWidget;
};
