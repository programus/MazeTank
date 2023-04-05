// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputControlComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class ABasePawn;
struct FInputActionValue;


class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANK_API UInputControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInputControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputMappingContext* InputContextMapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* MoveInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* AimInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* ShootInput;

	void BindInputMappingContext(AController* Controller);
	void BindActions(UInputComponent* PlayerInputComponent);

private:
	ABasePawn* Owner;
	APlayerController* PlayerController;

	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void AimByMouse();
};
