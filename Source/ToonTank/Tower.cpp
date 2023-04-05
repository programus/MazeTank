// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"

ATower::ATower()
{
	Initialize<UCapsuleComponent>();
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	RecoverWeapon();
}

void ATower::Tick(float DeltaTime)
{
	bNeedRotateTurret = Target && Target->IsActorTickEnabled();
	bool bTargetVisible = IsTargetInRange() && IsTargetVisible(Target);
	if (bNeedRotateTurret)
	{
		SetAimTargetLocation(Target->GetActorLocation());
	}
	CleanDisappearedProjectiles();
	bShootReady = bNeedRotateTurret && bTargetVisible && Projectiles.Num() < MaxProjectileNum && bWeaponReady;
	Super::Tick(DeltaTime);
	if (bShootReady)
	{
		RecoverWeapon();
	}
}

void ATower::Destruct()
{
	Super::Destruct();
	Destroy();
}

void ATower::RecoverWeapon()
{
	SetWeaponReady(false);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ATower::SetWeaponReady, true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, WeaponRecoverTime, false);
}

void ATower::SetWeaponReady(bool bReady)
{
	bWeaponReady = bReady;
}
