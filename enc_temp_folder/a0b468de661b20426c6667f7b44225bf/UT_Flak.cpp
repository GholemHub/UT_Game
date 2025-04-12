// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapon/UT_Flak.h"
#include "../Weapon/UT_Flak_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "../UT_GameCharacter.h"

#include "Engine/Engine.h"


// Sets default values
AUT_Flak::AUT_Flak()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//MeshComponent->SetupAttachment(RootComponent);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(MeshComponent);


}

// Called when the game starts or when spawned
void AUT_Flak::BeginPlay()
{
	Super::BeginPlay();
	if (MeshComponent)
	{

		OriginalRelativeLocation = MeshComponent->GetComponentLocation();
	}
}

// Called every frame
void AUT_Flak::Tick(float DeltaTime)
{
	if (bShouldRecover)
	{
		FVector CurrentLocation = MeshComponent->GetComponentLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, OriginalRelativeLocation, DeltaTime, 30.0f);
		MeshComponent->SetWorldLocation(NewLocation);

		if (FVector::Dist(NewLocation, OriginalRelativeLocation) < 0.1f)
		{
			MeshComponent->SetWorldLocation(OriginalRelativeLocation);
			bShouldRecover = false;
		}
	}
}

void AUT_Flak::Fire()
{
	
	if (!bCanFire) return;

	bCanFire = false;

	if (!FirePoint) return;
	FVector FirePointLocation = FirePoint->GetComponentLocation();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// Get screen center direction
	int32 ScreenWidth, ScreenHeight;
	PC->GetViewportSize(ScreenWidth, ScreenHeight);

	FVector2D ScreenCenter(ScreenWidth * 0.5f, ScreenHeight * 0.5f);
	FVector WorldLocation, WorldDirection;
	PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

	// Line trace from FirePoint to the direction of the screen center
	FVector EndTrace = FirePointLocation + (WorldDirection * 10000.0f); // 10,000 units long

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);  // Ignore self (weapon)
	TraceParams.AddIgnoredActor(GetOwner());  // Ignore weapon owner (player)

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FirePointLocation, EndTrace, ECC_Visibility, TraceParams);

	// Determine shooting direction
	FVector ShootDirection;
	if (bHit)
	{
		ShootDirection = (HitResult.ImpactPoint - FirePointLocation).GetSafeNormal();
	}
	else
	{
		ShootDirection = WorldDirection;  // Use forward direction if no hit
	}
	 
	if (!Projectile) return;

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		APawn* OwnerPawn = Cast<APawn>(OwnerActor);
		if (OwnerPawn)
		{
			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController && OwnerController->IsPlayerController())
			{
				auto Player = Cast<AUT_GameCharacter>(OwnerPawn);
				if (!Player) return;
				if (!GetWorld()) return;
				auto WEPON = Player->WeaponComponent->FlakWeapon;
				if (!WEPON) return;

				auto POINT = WEPON->FirePoint->GetComponentLocation();
				auto ROTATION = WEPON->FirePoint->GetComponentRotation();

				FTransform SpawnTransform(ROTATION, POINT);

				// Spawn the projectile properly
				auto Pr = GetWorld()->SpawnActorDeferred<AUT_Flak_Projectile>(Projectile, SpawnTransform);
				if (Pr)
				{
					Pr->FinishSpawning(SpawnTransform);
					Pr->MakeShot(ShootDirection);
					//MakeWeaponShake();
				}
				
			}
		}
	}
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &AUT_Flak::ResetFire, 1.0f, false);

}

void AUT_Flak::MakeWeaponShake()
{
	FVector ShakeOffset = FVector(
		FMath::RandRange(-3.0f, 1.0f),
		FMath::RandRange(-3.0f, 1.0f),
		FMath::RandRange(-1.0f, 1.0f)
	);
	//MeshComponent->AddLocalOffset(ShakeOffset);
	MeshComponent->SetWorldLocation(MeshComponent->GetComponentLocation() + ShakeOffset);

	bShouldRecover = 1;
}

void AUT_Flak::ResetFire()
{
	bCanFire = true;
	if (GetPlayer()->WeaponComponent->bIsShoot)
	{
		Fire();
	}
}

AUT_GameCharacter* AUT_Flak::GetPlayer()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		APawn* OwnerPawn = Cast<APawn>(OwnerActor);
		if (OwnerPawn)
		{
			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController && OwnerController->IsPlayerController())
			{
				auto Player = Cast<AUT_GameCharacter>(OwnerPawn);
				if (!Player) return nullptr;
				return Player;
			}
		}
	}
	return nullptr;
}
