// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "BasePawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::SetSpeed(float InitSpeed, float MaxSpeed)
{
	if (MoveComp)
	{
		MoveComp->InitialSpeed = InitSpeed;
		MoveComp->MaxSpeed = MaxSpeed;
	}
}

void AProjectile::SetDamage(float DamageValue)
{
	Damage = DamageValue;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.GetActor() != this)
	{
		AActor* OwnerActor = GetOwner();
		AController* InstigatorController = UKismetSystemLibrary::IsValid(OwnerActor) ? OwnerActor->GetInstigatorController() : nullptr;
		float RealDamage = Damage *= FMath::RandRange(1 - DamageFluctuation, 1 + DamageFluctuation);
		UGameplayStatics::ApplyDamage(OtherActor, Damage, InstigatorController, this, UDamageType::StaticClass());
		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				HitEffect,
				Hit.ImpactPoint
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
		}
		if (HitCameraShakeClass)
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
		Destroy();
	}
}

