// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Asteroid.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Projectile Mesh");
	RootComponent = StaticMesh;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision Box");
	CapsuleComponent->SetupAttachment(StaticMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.5f;

	this->OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor))
	{
		Asteroid->HandleDamage(Damage);
		Destroy();
	}
}
