// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon/UT_Flak_Projectile.h"
#include "UT_Redeemer_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class UT_GAME_API AUT_Redeemer_Projectile : public AUT_Flak_Projectile
{
	GENERATED_BODY()
public:
	virtual void MakeShot(FVector ShootDirection) override;
};
