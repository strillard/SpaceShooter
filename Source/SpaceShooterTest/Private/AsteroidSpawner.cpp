// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidSpawner.h"

// Sets default values
AAsteroidSpawner::AAsteroidSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawnRateTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, FMath::FRandRange(MinSpawnRate, MaxSpawnRate), false);
	
}

// Called every frame
void AAsteroidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAsteroidSpawner::SpawnAsteroid()
{
	ASpaceShip* PlayerRef = Cast<ASpaceShip>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (PlayerRef)
	{
		FVector PlayerForwardVector = PlayerRef->GetActorForwardVector();
		float Angle = FMath::RandRange(-180.f, 180.f);
		FRotator Rot(0.f, Angle, 0.f);
		FVector Dir = Rot.RotateVector(PlayerForwardVector);

		Dir = Dir.GetSafeNormal() * 1000.f;
		FVector SpawnPoint = PlayerRef->GetActorLocation() + Dir;
		GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, SpawnPoint, GetActorRotation());

		GetWorld()->GetTimerManager().SetTimer(SpawnRateTimerHandle, this, &AAsteroidSpawner::SpawnAsteroid, FMath::FRandRange(MinSpawnRate, MaxSpawnRate), false);
	}
}

