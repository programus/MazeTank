// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.h"
#include "VisualHealthComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	TurretRotationOffset = TurretComp->GetComponentRotation().Yaw - GetActorRotation().Yaw;
	WeaponRangeSqr = WeaponRange * WeaponRange;

	LastMoveSpeed.X = LastMoveSpeed.Y = LastMoveSpeed.Z = 0;
	LastRotation.Yaw = LastRotation.Roll = LastRotation.Pitch = 0;
}

void ABasePawn::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("%s shoot!!"), *GetActorNameOrLabel());
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentTransform());
	Projectile->SetOwner(this);
	Projectile->SetSpeed(ProjectileInitSpeed, ProjectileMaxSpeed);
	Projectile->SetDamage(WeaponDamage);
	CleanDisappearedProjectiles();
	Projectiles.Add(Projectile);
	bShootReady = false;
	if (ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, ProjectileSpawnPoint->GetComponentLocation());
	}
}

void ABasePawn::TurnTurret()
{
	FVector Direction = AimTargetLocation - GetActorLocation();
	FRotator Rotation = Direction.Rotation();
	Rotation.Roll = 0;
	Rotation.Pitch = 0;
	Rotation.Yaw += TurretRotationOffset;
	TurretComp->SetWorldRotation(
		FMath::RInterpTo(
			TurretComp->GetComponentRotation(), 
			Rotation, 
			GetWorld()->GetDeltaSeconds(),
			TurretMaxRotateSpeed
		), 
		true
	);
}

void ABasePawn::MoveBase()
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector InputDirection(MoveInput.Y, MoveInput.X, 0);
	if (APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager)
	{
		FRotator CameraRotation = CameraManager->GetCameraRotation();
		CameraRotation.Roll = CameraRotation.Pitch = 0;

		FVector Direction = CameraRotation.RotateVector(InputDirection);
		Direction.Normalize();

		FVector MoveDir = Direction.ProjectOnTo(GetActorForwardVector());
		AddActorWorldOffset(LastMoveSpeed = FMath::VInterpTo(
			LastMoveSpeed,
			MoveDir * MoveSpeed,
			DeltaTime,
			MoveAcceleration
		), true);

		FRotator Rotation = Direction.Rotation();
		FRotator BodyRotation = GetActorRotation();
		float DeltaAngle = 0;
		if (!MoveInput.IsNearlyZero())
		{
			if (FMath::IsNearlyZero(MoveInput.Y))
			{
				DeltaAngle = FMath::FindDeltaAngleDegrees(CameraRotation.Yaw, BodyRotation.Yaw);
				if (DeltaAngle * MoveInput.X < 0 && FMath::Abs(DeltaAngle) > 45)
				{
					Rotation.Yaw += 180;
				}
			}
			else
			{
				if (MoveInput.Y < 0)
				{
					Rotation.Yaw += 180;
				}
			}
			LastRotation = Rotation;
		}
		SetActorRotation(FMath::RInterpTo(
			BodyRotation,
			LastRotation,
			DeltaTime,
			RotateSpeed
		));
	}
}

bool ABasePawn::IsTargetInRange()
{
	float DistSqr = FVector::DistSquared(GetActorLocation(), AimTargetLocation);
	
	return DistSqr <= WeaponRangeSqr;
}

bool ABasePawn::IsTargetVisible(AActor* Target)
{
	bool ret = false;
	FVector StartPoint = ProjectileSpawnPoint->GetComponentLocation();
	FVector EndPoint = StartPoint + ProjectileSpawnPoint->GetForwardVector() * FVector::DistSquared(AimTargetLocation, StartPoint);
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, EndPoint, ECC_Visibility))
	{
		ret = Hit.GetActor() == Target;
	}
	return ret;
}

void ABasePawn::CleanDisappearedProjectiles()
{
	Projectiles.RemoveAll([](const AProjectile* P)
		{
			return !UKismetSystemLibrary::IsValid(P);
		});
}

void ABasePawn::Destruct()
{
	FVector Location(GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		DestroyEffect,
		Location
	);
	if (DestructSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestructSound, Location);
	}
	if (DestructionShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestructionShakeClass);
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanMove)
	{
		MoveBase();
	}
	TurnTurret();
	if (bShootReady)
	{
		Shoot();
	}
}

void ABasePawn::SetMoveInput(const FVector2D Input)
{
	MoveInput = Input;
}

void ABasePawn::SetAimTargetLocation(const FVector Input)
{
	AimTargetLocation = Input;
}

void ABasePawn::SetShootReady(bool bReady)
{
	bShootReady = bReady;
}

void ABasePawn::BuildStructure()
{
	BaseComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	BaseComp->SetupAttachment(RootComponent);

	TurretComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	TurretComp->SetupAttachment(BaseComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretComp);

	HealthBarComp = CreateDefaultSubobject<UVisualHealthComponent>(TEXT("Health"));
	HealthBarComp->SetupAttachment(RootComponent);
}

