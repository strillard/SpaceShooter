// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpaceShip::ASpaceShip()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	ProjectileSpawnPoint1 = CreateDefaultSubobject<USceneComponent>("Projectile Spawn Point 1");
	ProjectileSpawnPoint2 = CreateDefaultSubobject<USceneComponent>("Projectile Spawn Point 2");

	StaticMesh->SetupAttachment(GetCapsuleComponent());
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	ProjectileSpawnPoint1->SetupAttachment(StaticMesh);
	ProjectileSpawnPoint2->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		int32 ScreenX, ScreenY;
		GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);

		FVector WorldLocation, WorldDirection;
		GetWorld()->GetFirstPlayerController()->DeprojectScreenPositionToWorld(
			ScreenX, ScreenY, WorldLocation, WorldDirection);

		PlayerController->bShowMouseCursor = true;

		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetIgnoreLookInput(true);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		float MouseX, MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);

		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 8, FColor(1, 0, 0));

		Rotate(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASpaceShip::Move);
		EnhancedInputComponent->BindAction(IA_Scroll, ETriggerEvent::Triggered, this, &ASpaceShip::Scroll);
		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ASpaceShip::Fire);
	}
}

void ASpaceShip::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const FVector Right = FVector::RightVector;
		AddMovementInput(Right, MovementValue.X);

		const FVector Forward = FVector::ForwardVector;
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void ASpaceShip::Scroll(const FInputActionValue& Value)
{
	float FloatValue = Value.Get<float>();

	if (Controller && SpringArm)
	{
		float Scroll = FloatValue * ScrollSensitivity;
		SpringArm->TargetArmLength += Scroll;
		SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, MinScrollDist, MaxScrollDist);
	}
}

void ASpaceShip::Rotate(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - GetActorLocation();
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw, 0.0f);

	SetActorRotation(LookAtRotation);
}

void ASpaceShip::Fire()
{
	if (CanFire)
	{
		CanFire = false;

		FRotator Rotation = GetActorRotation();
		Rotation.Roll = -90.f;

		if (FireRightSpawnPoint)
		{
			GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint1->GetComponentLocation(),
			                                    GetActorRotation());
		}
		else
		{
			GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint2->GetComponentLocation(),
			                                    GetActorRotation());

		}
		FireRightSpawnPoint = !FireRightSpawnPoint;

		GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ASpaceShip::ResetFire, FireRate, false);
	}
}

void ASpaceShip::ResetFire()
{
	CanFire = true;
}

void ASpaceShip::AddScore(int ScoreToAdd)
{
	Score += ScoreToAdd;
}

void ASpaceShip::HandleDamage(int DamageTaken)
{
	CurrentHealth -= DamageTaken;

	if (CurrentHealth <= 0)
	{
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		UGameplayStatics::OpenLevel(this, FName("MainMenuLevel"));
	}
}
