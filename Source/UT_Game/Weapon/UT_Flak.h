// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Weapon/UT_Flak_Projectile.h"


#include "UT_Flak.generated.h"

UCLASS()
class UT_GAME_API AUT_Flak : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUT_Flak();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* FirePoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AUT_Flak_Projectile> Projectile;
	void Fire();

private:

};
