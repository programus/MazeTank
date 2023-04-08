// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputControlComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class ATank;
class USpringArmComponent;
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
		UInputAction* AutoAimInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		UInputAction* ShootInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float RotateSpeed = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float ViewSpeed = 90;
	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;

	void BindInputMappingContext(AController* Controller);
	void BindActions(UInputComponent* PlayerInputComponent);

private:
	ATank* Owner;
	APlayerController* PlayerController;

	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void Aim(const FInputActionValue& Value);
	void AimEnd(const FInputActionValue& Value);
	void AutoAim(const FInputActionValue& Value);
	void AutoAimEnd(const FInputActionValue& Value);
	void AimByMouse();
};
