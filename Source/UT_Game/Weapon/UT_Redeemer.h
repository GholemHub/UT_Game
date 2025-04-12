// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon/UT_Flak.h"
#include "UT_Redeemer.generated.h"

/**
 * 
 */
UCLASS()
class UT_GAME_API AUT_Redeemer : public AUT_Flak
{
	GENERATED_BODY()

public:


	virtual void Fire() override;
	virtual void ResetFire() override;
};
