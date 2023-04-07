// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComp->SetMobility(EComponentMobility::Static);

	FloorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	FloorComp->SetupAttachment(RootComp);
	FloorComp->SetMobility(EComponentMobility::Static);

	WallComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	WallComp->SetupAttachment(RootComp);
	WallComp->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWall::HideWall()
{
	WallComp->SetVisibility(false);
	WallComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

