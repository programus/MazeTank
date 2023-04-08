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

void ATank::SetAutoAim(bool bEnabled)
{
	bAutoAim = bEnabled;
	bRotateToTarget = bEnabled;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::Tick(float DeltaTime)
{
	if (bAutoAim)
	{
		AutoAim();
	}
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

void ATank::AutoAim()
{
	FHitResult Hit;
	FCollisionShape Sphere;
	Sphere.SetSphere(AutoAimSensorRadius);
	FVector Start(ProjectileSpawnPoint->GetComponentLocation());
	FVector Direction = ProjectileSpawnPoint->GetComponentRotation().Vector();
	Start += Direction * AutoAimSensorRadius;
	FVector End = Start + Direction * WeaponRange;
	FCollisionQueryParams QueryParams(FName(TEXT("Trace Enemy")), false, this);
	if (GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, FCollisionObjectQueryParams(ECC_Pawn), Sphere, QueryParams))
	{
		// found an enemy.
		bRotateToTarget = true;
		AimTargetLocation = Hit.GetActor()->GetActorLocation();
	}
	else
	{
		bRotateToTarget = false;
	}
}

