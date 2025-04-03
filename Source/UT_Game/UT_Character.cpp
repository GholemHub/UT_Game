// Fill out your copyright notice in the Description page of Project Settings.


#include "UT_Character.h"
#include "Weapon/UT_WeaponComponent.h"

#include "Engine/Engine.h"

// Sets default values
// Constructor
AUT_Character::AUT_Character()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set Root Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Attach Camera to Root
    //FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    //FollowCamera->SetupAttachment(RootComponent);

    // Attach Weapon Point to Camera
   // WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachPoint"));
    //WeaponAttachPoint->SetupAttachment(FollowCamera);

    //WeaponComponent = CreateDefaultSubobject<UUT_WeaponComponent>(TEXT("WeaponComponent"));
}


// Called when the game starts or when spawned
void AUT_Character::BeginPlay()
{
	Super::BeginPlay();

    //if (!WeaponComponent) return;
    //WeaponComponent->EquipWeapon(this);

   
}

// Called every frame
void AUT_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUT_Character::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AUT_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUT_Character::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUT_Character::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUT_Character::Move);
    }
    else
    {
        //UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}

void AUT_Character::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add movement 
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AUT_Character::Fire(const FInputActionValue& Value)
{
    //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("BooM"));
   /* auto Weapon = Cast<UUT_WeaponComponent>(WeaponComponent);
    if (!Weapon) return;
    Weapon->FireStart();*/
}




