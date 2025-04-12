// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapon/UT_Redeemer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "../UT_GameCharacter.h"


void AUT_Redeemer::Fire()
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
	GetWorldTimerManager().SetTimer(FireRateHandle, this, &AUT_Redeemer::ResetFire, 0.1f, false);
}

void AUT_Redeemer::ResetFire()
{
	bCanFire = true;
	if (GetPlayer()->WeaponComponent->bIsShoot)
	{
		Fire();
	}
}
