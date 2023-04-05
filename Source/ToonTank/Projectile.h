// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class ABasePawn;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UCameraShake;

UCLASS()
class TOONTANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetSpeed(float InitSpeed, float MaxSpeed);
	void SetDamage(float DamageValue);

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere)
		UProjectileMovementComponent* MoveComp;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float Damage = 30;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float DamageFluctuation = 0.5;
	UPROPERTY(EditAnywhere, Category = "Combat")
		UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> HitCameraShakeClass;
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	ABasePawn* Launcher;
};
