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
	float VelocityMultiplier = GetWorld()->DeltaTimeSeconds * 100.f * MovementSpeed;
	CurrentVelocity.X = FMath::Clamp(Axis, -1.f, 1.f) * VelocityMultiplier;
	SetActorLocation(GetActorLocation() + CurrentVelocity);
}

void ABomberman::MoveRight(float Axis)
{
	float VelocityMultiplier = GetWorld()->DeltaTimeSeconds * 100.f * MovementSpeed;
	CurrentVelocity.Y = FMath::Clamp(Axis, -1.f, 1.f) * VelocityMultiplier;
	SetActorLocation(GetActorLocation() + CurrentVelocity);
}

void ABomberman::SpawnBomb()
{
	if (BombCapacity > 0)
	{
		// Setting location of bomb spawn to the nearest hundred
		FVector BombSpawnLocation = GetBombSpawnLocation();

		// Spawn a bomb and store a reference to that bomb
		FActorSpawnParameters SpawnParams;
		ABomb* SpawnedActorRef = GetWorld()->SpawnActor<ABomb>(
			BombToSpawn, 
			BombSpawnLocation, 
			GetActorRotation(), 
			SpawnParams
		);

		// Set bomb's explosion range to the explosion range of the player
		SpawnedActorRef->BombRangeMultiplier = BombRange;
		// Add a reference to the bomb to the player's bomb spawned list
		BombsSpawned.Emplace(SpawnedActorRef);
		// Reduce the number of bombs the player can place down
		BombCapacity--;
	}
	else // Remove any bombs that have blown up and replenish player's bomb capacity
	{
		RemoveDespawnedBombs();		
	}
}

FVector ABomberman::GetBombSpawnLocation() const
{
	float BombSpawnX = FMath::DivideAndRoundNearest(GetActorLocation().X, 100.f);
	float BombSpawnY = FMath::DivideAndRoundNearest(GetActorLocation().Y, 100.f);

	int BombSpawnXToGrid = BombSpawnX;
	BombSpawnXToGrid *= 100;
	int BombSpawnYToGrid = BombSpawnY;
	BombSpawnYToGrid *= 100;

	return FVector(BombSpawnXToGrid, BombSpawnYToGrid, GetActorLocation().Z);
}

void ABomberman::RemoveDespawnedBombs()
{
	// Checks only if the first bomb in the list has despawned as they are
	// appended by earliest bomb placed
	if (BombsSpawned[0]->TimeSinceSpawned > BombsSpawned[0]->ExplosionDelay)
	{
		// If the oldest bomb has despawned, replenish capacity and call SpawnBomb()
		BombsSpawned.RemoveAt(0, 1, true);		
		BombCapacity++;
		SpawnBomb();
	}
}