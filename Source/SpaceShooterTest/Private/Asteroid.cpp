// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

void AAsteroid::OnStop(const FHitResult& ImpactResult)
{
	UE_LOG(LogTemp, Log, TEXT("Asteroide a rebondi STOP contre %s"), *ImpactResult.GetActor()->GetName());

	if (IsValid(this) && GetWorld())
	{
		this->Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asteroide cannot be destroyed because it doesn't have a valid world pointer."));
	}

}

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Collision Box");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	ProjectileMovementComponent->InitialSpeed = FMath::FRandRange(MinSpeed, MaxSpeed);
	ProjectileMovementComponent->MaxSpeed = FMath::FRandRange(MinSpeed, MaxSpeed);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->BounceAdditionalIterations = 1;
	ProjectileMovementComponent->Bounciness = 1.f;
	ProjectileMovementComponent->Friction = 0.0f;

	ProjectileMovementComponent->SetPlaneConstraintEnabled(true);
	ProjectileMovementComponent->SetPlaneConstraintNormal(FVector::UpVector);


	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement Component");

	RootComponent = StaticMesh;
	CapsuleComponent->SetupAttachment(StaticMesh);

	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AAsteroid::OnProjectileBounceHandler);
	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AAsteroid::OnStop);

	this->OnActorBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlap);

	this->InitialLifeSpan = 20.f;
}
// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = FMath::RandRange(MinHealth, MaxHealth);

	PlayerRef = Cast<ASpaceShip>(GetWorld()->GetFirstPlayerController()->GetPawn());
	RotatingMovementComponent->RotationRate = FRotator(FMath::RandRange(0.1, 45.0), FMath::RandRange(0.1, 45.0), FMath::RandRange(0.1, 45.0));

	if (PlayerRef)
	{
	    MovementDirection = (PlayerRef->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		if (ProjectileMovementComponent)
		{
			ProjectileMovementComponent->Velocity = MovementDirection * ProjectileMovementComponent->InitialSpeed;
		}
	}
}

void AAsteroid::OnProjectileBounceHandler(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	UE_LOG(LogTemp, Log, TEXT("Asteroide a rebondi contre %s"), *ImpactResult.GetActor()->GetName());

}

void AAsteroid::HandleDamage(int DamageTaken)
{
	CurrentHealth -= DamageTaken;

	if (CurrentHealth <= 0)
	{
		HandleDestruction();
	}
}

void AAsteroid::HandleDestruction()
{
	PlayerRef->AddScore(Score);
	Destroy();
}

void AAsteroid::OnOverlap(AActor* MyActor, AActor* OtherActor)
{
	if (ASpaceShip* Player = Cast<ASpaceShip>(OtherActor))
	{
		Player->HandleDamage(Damage);
		Destroy();
	}
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

