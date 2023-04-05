// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UVisualHealthComponent;
class UShapeComponent;
class AProjectile;
class UNiagaraSystem;
class UCameraShakeBase;

UCLASS()
class TOONTANK_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	template<class TCollisionComponentType>
	void Initialize()
	{
		FString TypeName(typeid(TCollisionComponentType).name());
		FString Name = FString::Format(TEXT("{0} Collision"), { TypeName });
		RootComponent = CollisionComp = CreateDefaultSubobject<TCollisionComponentType>(FName(*Name));
		BuildStructure();
	}

	virtual void Shoot();
	virtual void TurnTurret();
	virtual void MoveBase();
	virtual bool IsTargetInRange();
	virtual bool IsTargetVisible(AActor* Target);

	void CleanDisappearedProjectiles();

	TArray<AProjectile*> Projectiles;

	FVector AimTargetLocation;
	FVector2D MoveInput;
	bool bShootReady;
	bool bNeedRotateTurret;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMoveInput(const FVector2D Input);
	void SetAimTargetLocation(const FVector Input);
	void SetShootReady(bool bReady);

	virtual void Destruct();

private:
	UPROPERTY(VisibleAnywhere)
		UShapeComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BaseComp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TurretComp;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere)
		UVisualHealthComponent* HealthBarComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponRange = 1000;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponDamage = 30;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float ProjectileInitSpeed = 1000;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float ProjectileMaxSpeed = 1000;
	UPROPERTY(EditAnywhere, Category = "Combat")
		UNiagaraSystem* DestroyEffect;
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* DestructSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> DestructionShakeClass;
	UPROPERTY(EditAnywhere, Category = "Moving")
		float TurretMaxRotateSpeed = 15;
	UPROPERTY(EditAnywhere, Category = "Moving")
		bool bCanMove = false;
	UPROPERTY(EditAnywhere, Category = "Moving")
		float MoveSpeed = 5;
	UPROPERTY(EditAnywhere, Category = "Moving")
		float RotateSpeed = 3;
	UPROPERTY(EditAnywhere, Category = "Moving")
		float MaxRotateSpeed = 6;
	UPROPERTY(EditAnywhere, Category = "Moving")
		float MoveAcceleration = 5;

	void BuildStructure();
	float TurretRotationOffset;
	float WeaponRangeSqr;

	FVector LastMoveSpeed;
	FRotator LastRotation;
};
