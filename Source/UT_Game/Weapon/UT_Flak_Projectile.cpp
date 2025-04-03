#include "Components/StaticMeshComponent.h"
#include "../Weapon/UT_Flak_Projectile.h"
#include "Engine/Engine.h"


// Sets default values
AUT_Flak_Projectile::AUT_Flak_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	MeshComponent->OnComponentHit.AddDynamic(this, &AUT_Flak_Projectile::OnHit);  // Bind collision event


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f; // Adjust speed
	ProjectileMovement->MaxSpeed = 6000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AUT_Flak_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUT_Flak_Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

        Destroy();
    
}

// Called every frame
void AUT_Flak_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUT_Flak_Projectile::MakeShot(FVector ShootDirection)
{
    if (MeshComponent) // Ensure MeshComponent exists
    {
        if (ShootDirection.IsNearlyZero())
        {
            ShootDirection = GetActorForwardVector();
        }
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Shot Direction: %s"), *ShootDirection.ToString()));
        FVector Impulse = ShootDirection * 1000.0f; // Adjust impulse strength

        MeshComponent->AddImpulse(Impulse, NAME_None, true); // Apply impulse to the mesh
    }

}