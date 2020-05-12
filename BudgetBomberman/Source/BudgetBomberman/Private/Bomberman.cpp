// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomberman.h"
#include "Bomb.h"

// Sets default values
ABomberman::ABomberman()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Controller does not control pawn rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABomberman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABomberman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Handle movement based on our "MoveForward" and "MoveRight" axes
    {
        if (!CurrentVelocity.IsZero())
        {
            FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
            SetActorLocation(NewLocation);
        }
    }
}

// Called to bind functionality to input
void ABomberman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this, &ABomberman::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABomberman::MoveRight);

	PlayerInputComponent->BindAction("PlaceBomb", IE_Pressed, this, &ABomberman::SpawnBomb);

}

void ABomberman::MoveForward(float Axis)
{
	CurrentVelocity.X = FMath::Clamp(Axis, -1.f, 1.f) * 100.f * MovementSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Forward Axis: %f"), Axis);
}

void ABomberman::MoveRight(float Axis)
{
	CurrentVelocity.Y = FMath::Clamp(Axis, -1.f, 1.f) * 100.f * MovementSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Right Axis: %f"), Axis);
}

void ABomberman::SpawnBomb()
{
	float BombSpawnX = FMath::DivideAndRoundNearest(GetActorLocation().X, 100.f);
	float BombSpawnY = FMath::DivideAndRoundNearest(GetActorLocation().Y, 100.f);

	int BombSpawnXToGrid = BombSpawnX;
	BombSpawnXToGrid *= 100;
	int BombSpawnYToGrid = BombSpawnY;
	BombSpawnYToGrid *= 100;

	FVector BombSpawnLocation(
		BombSpawnXToGrid,
		BombSpawnYToGrid,
		GetActorLocation().Z
	);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *BombSpawnLocation.ToCompactString());

	FActorSpawnParameters SpawnParams;
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(
		BombToSpawn, 
		BombSpawnLocation, 
		GetActorRotation(), 
		SpawnParams
	);
}