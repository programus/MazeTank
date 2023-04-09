// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "ToonTankGameModeBase.h"

// Sets default values
AGoalTrigger::AGoalTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = TriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AGoalTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AGoalTrigger::OnOverlapBegin);
}

// Called every frame
void AGoalTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Goal triggered by %s"), *OtherActor->GetActorNameOrLabel());
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (AToonTankGameModeBase* GameMode = Cast<AToonTankGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->EndGame(true);
		}
	}
}

