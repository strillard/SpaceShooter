// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Projectile.h"
#include "SpaceShip.generated.h"

UCLASS()
class SPACESHOOTERTEST_API ASpaceShip : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint1;

	UPROPERTY(EditAnywhere)
	USceneComponent* ProjectileSpawnPoint2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Scroll;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Shoot;

	UPROPERTY(EditAnywhere)
	float ScrollSensitivity = 50;

	UPROPERTY(EditAnywhere)
	float MaxScrollDist = 500.0f;

	UPROPERTY(EditAnywhere)
	float MinScrollDist = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;

	FTimerHandle FireRateTimerHandle;
	bool CanFire = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Scroll(const FInputActionValue& Value);

	void Rotate(FVector LookAtTarget);

	void Fire();

	void ResetFire();

	void AddScore(int ScoreToAdd);

	bool FireRightSpawnPoint = true;

	UPROPERTY(BlueprintReadOnly)
	int Score = 0;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.1;

	UPROPERTY(EditAnywhere)
	int MaxHealth = 3;

	UPROPERTY(BlueprintReadOnly)
	int CurrentHealth;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	void HandleDamage(int DamageTaken);

};