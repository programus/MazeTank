// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Components/BoxComponent.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "InputControlComponent.h"

ATank::ATank()
{
	Initialize<UBoxComponent>();
}

void ATank::Destruct()
{
	Super::Destruct();
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (GetInputControlComp())
	{
		GetInputControlComp()->BindActions(PlayerInputComponent);
	}
}

void ATank::PawnClientRestart()
{
	Super::PawnClientRestart();
	if (GetInputControlComp())
	{
		GetInputControlComp()->BindInputMappingContext(GetController());
	}
}

UInputControlComponent* ATank::GetInputControlComp()
{
	if (!InputControlComp)
	{
		InputControlComp = Cast<UInputControlComponent>(GetComponentByClass(UInputControlComponent::StaticClass()));
	}
	return InputControlComp;
}

