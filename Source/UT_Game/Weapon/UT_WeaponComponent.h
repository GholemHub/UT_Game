// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UT_WeaponComponent.generated.h"

class AUT_Flak;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UT_GAME_API UUT_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUT_WeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EquipWeapon(ACharacter*);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<AUT_Flak> Weapon;
	UPROPERTY()
	AUT_Flak* FlakWeapon;
	void FireStart();
private:
	
};
