// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpaceShip.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Asteroid.generated.h"

UCLASS()
class SPACESHOOTERTEST_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnStop(const FHitResult& ImpactResult);
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;

	UFUNCTION()
	void OnProjectileBounceHandler(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	void HandleDestruction();

	UPROPERTY(EditAnywhere)
	int MaxHealth = 20;

	UPROPERTY(EditAnywhere)
	int MinHealth = 5;

	int CurrentHealth;

	UPROPERTY(EditAnywhere)
	float MinSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 400.f;

	UPROPERTY(EditAnywhere)
	int Damage = 1;

	UPROPERTY(EditAnywhere)
	int Score = 10;

	ASpaceShip* PlayerRef;

	FVector MovementDirection;

	UFUNCTION()
	void OnOverlap(AActor* MyActor, AActor* OtherActor);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDamage(int DamageTaken);

};
