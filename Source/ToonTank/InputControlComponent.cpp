// Fill out your copyright notice in the Description page of Project Settings.


#include "InputControlComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tank.h"

// Sets default values for this component's properties
UInputControlComponent::UInputControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInputControlComponent::BeginPlay()
{
	Super::BeginPlay();

    Owner = Cast<ATank>(GetOwner());
    SpringArm = Cast<USpringArmComponent>(Owner->GetComponentByClass(USpringArmComponent::StaticClass()));
}


// Called every frame
void UInputControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    //AimByMouse();
}

void UInputControlComponent::BindInputMappingContext(AController* Controller)
{
    if (APlayerController* PC = PlayerController = Cast<APlayerController>(Controller))
    {
        // Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
            Subsystem->ClearAllMappings();

            // Add each mapping context, along with their priority values. Higher values outprioritize lower values.
            Subsystem->AddMappingContext(InputContextMapping, 0);
        }
    }
}

void UInputControlComponent::BindActions(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComp->BindAction(MoveInput, ETriggerEvent::Triggered, this, &UInputControlComponent::Move);
        EnhancedInputComp->BindAction(MoveInput, ETriggerEvent::Completed, this, &UInputControlComponent::MoveEnd);
        EnhancedInputComp->BindAction(AimInput, ETriggerEvent::Triggered, this, &UInputControlComponent::Aim);
        EnhancedInputComp->BindAction(AimInput, ETriggerEvent::Completed, this, &UInputControlComponent::AimEnd);
        EnhancedInputComp->BindAction(AutoAimInput, ETriggerEvent::Started, this, &UInputControlComponent::AutoAim);
        EnhancedInputComp->BindAction(AutoAimInput, ETriggerEvent::Completed, this, &UInputControlComponent::AutoAimEnd);
        EnhancedInputComp->BindAction(ShootInput, ETriggerEvent::Started, this, &UInputControlComponent::Shoot);
    }
}

void UInputControlComponent::Move(const FInputActionValue& Value)
{
    Owner->SetMoveInput(Value.Get<FVector2D>());
}

void UInputControlComponent::MoveEnd(const FInputActionValue& Value)
{
    Owner->SetMoveInput(FVector2D::ZeroVector);
}

void UInputControlComponent::Aim(const FInputActionValue& Value)
{
	FVector2D Input(Value.Get<FVector2D>());
    Owner->SetAimInput(Input * RotateSpeed);

    if (SpringArm)
    {
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        FRotator Rotation(SpringArm->GetRelativeRotation());
        Rotation.Pitch += Input.Y * ViewSpeed * DeltaTime;
        if (Rotation.Pitch > 0)
        {
            Rotation.Pitch = 0;
        }
        if (Rotation.Pitch < -90)
        {
            Rotation.Pitch = -90;
        }
		SpringArm->SetRelativeRotation(Rotation);
    }
}

void UInputControlComponent::AimEnd(const FInputActionValue& Value)
{
    Owner->SetAimInput(FVector2D::ZeroVector);
}

void UInputControlComponent::AutoAim(const FInputActionValue& Value)
{
    Owner->SetAutoAim(true);
}

void UInputControlComponent::AutoAimEnd(const FInputActionValue& Value)
{
    Owner->SetAutoAim(false);
}

void UInputControlComponent::AimByMouse()
{
    if (PlayerController && PlayerController->GetPawn()->InputEnabled())
    {
        FHitResult Hit;
        if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
        {
            if (Hit.GetActor() != PlayerController->GetPawn())
            {
				Owner->SetAimTargetLocation(Hit.ImpactPoint);
            }
        }
    }
}

void UInputControlComponent::Shoot(const FInputActionValue& Value)
{
    Owner->SetShootReady(Value.Get<bool>());
}

