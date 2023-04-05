// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTankGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidget.h"

// Sets default values for this component's properties
UVisualHealthComponent::UVisualHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	this->SetWidgetSpace(EWidgetSpace::World);
	this->SetCollisionProfileName(TEXT("OverlapAll"));
}


// Called when the game starts
void UVisualHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	GameMode = Cast<AToonTankGameModeBase>(UGameplayStatics::GetGameMode(this));
	HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UVisualHealthComponent::OnDamaged);
}


// Called every frame
void UVisualHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType != LEVELTICK_ViewportsOnly)
	{
		APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector Start = GetComponentLocation();
		FVector End = CameraManager->GetCameraLocation();
		FVector Direction = End - Start;

		SetWorldRotation(Direction.Rotation());
	}
}

void UVisualHealthComponent::OnDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth -= Damage;
	HealthBarWidget->SetHealth(CurrentHealth, MaxHealth);
	if (CurrentHealth <= 0 && GameMode)
	{
		GameMode->KillActor(DamagedActor);
	}
}
