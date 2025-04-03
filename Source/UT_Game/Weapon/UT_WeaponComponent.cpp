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
}


// Called every frame
void UUT_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}
void UUT_WeaponComponent::FireStart()
{
    /*if (!bCanFire) return;
    bCanFire = false;*/
   
    
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
    if (!Player) return;  // Check if the Player is valid

    // Debugging WeaponAttachPoint
    if (!Player->WeaponAttachPoint)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("WeaponAttachPoint is NULL!"));
        return;
    }

    // Debugging spawn location
    FVector SpawnLocation = Player->WeaponAttachPoint->GetComponentLocation();
    FRotator SpawnRotation = Player->WeaponAttachPoint->GetComponentRotation();

    // Add debug message to verify spawn location and rotation
    FString LocationMessage = FString::Printf(TEXT("SpawnLocation: %s"), *SpawnLocation.ToString());
    FString RotationMessage = FString::Printf(TEXT("SpawnRotation: %s"), *SpawnRotation.ToString());

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, LocationMessage);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, RotationMessage);

    // Spawn weapon at WeaponAttachPoint location
    FlakWeapon = GetWorld()->SpawnActor<AUT_Flak>(Weapon, SpawnLocation, SpawnRotation, SpawnParams);

    if (!FlakWeapon) return;

    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("3"));

    // Attach the weapon to WeaponAttachPoint after spawning it
    FlakWeapon->AttachToComponent(Player->WeaponAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    FlakWeapon->SetActorHiddenInGame(false);
    FlakWeapon->MeshComponent->SetVisibility(true);

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("4"));
}
