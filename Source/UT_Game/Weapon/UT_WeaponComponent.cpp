// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapon/UT_WeaponComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapon/UT_Flak.h"
#include "../UT_GameCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UUT_WeaponComponent::UUT_WeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//FlakWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	// ...
}


// Called when the game starts
void UUT_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
  
  

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return; // Fixed missing semicolon



	EquipWeapon(OwnerCharacter);

    AUT_GameCharacter* Player = Cast<AUT_GameCharacter>(GetOwner());
    if (Player)
    {
        Player->OnFirePressed.AddDynamic(this, &UUT_WeaponComponent::HandleFirePressed);
        Player->OnFireReleased.AddDynamic(this, &UUT_WeaponComponent::HandleFireReleased);
    }
}

void UUT_WeaponComponent::HandleFirePressed()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DELEGATE 1"));
    bIsShoot = true;
    FireStart();
}

void UUT_WeaponComponent::HandleFireReleased()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("DELEGATE 2"));
    bIsShoot = false;
    // Optional: Logic to stop shooting, stop charging, etc.
}
// Called every frame
void UUT_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}
void UUT_WeaponComponent::FireStart()
{
    if (!FlakWeapon) return;
    FlakWeapon->Fire();   
}

void UUT_WeaponComponent::EquipWeapon(ACharacter* Owner)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("1"));

    if (!Weapon || !Owner) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Owner;
    SpawnParams.Instigator = Owner->GetInstigator();

    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("2"));
    auto Player = Cast<AUT_GameCharacter>(Owner);
    if (!Player) return;

    if (!Player->WeaponAttachPoint) return;

    // Debugging spawn location
    FVector SpawnLocation = Player->WeaponAttachPoint->GetComponentLocation();
    FRotator SpawnRotation = Player->WeaponAttachPoint->GetComponentRotation();

    FlakWeapon = GetWorld()->SpawnActor<AUT_Flak>(Weapon, SpawnLocation, SpawnRotation, SpawnParams);

    if (!FlakWeapon) return;

    FlakWeapon->AttachToComponent(Player->WeaponAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    FlakWeapon->SetActorHiddenInGame(false);
    FlakWeapon->MeshComponent->SetVisibility(true);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("4"));
}
