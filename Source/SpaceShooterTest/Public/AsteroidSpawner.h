// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asteroid.h"
#include "GameFramework/Actor.h"
#include "AsteroidSpawner.generated.h"

UCLASS()
class SPACESHOOTERTEST_API AAsteroidSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float MinSpawnRate = 1; // 1 par secondes

	UPROPERTY(EditAnywhere)
	float MaxSpawnRate = 4; // 1 toutes les 4 secondes

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAsteroid> AsteroidClass;

private:
	FTimerHandle SpawnRateTimerHandle;

	void SpawnAsteroid();

};
