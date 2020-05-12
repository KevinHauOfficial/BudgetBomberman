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

		FActorSpawnParameters SpawnParams;
		ABomb* SpawnedActorRef = GetWorld()->SpawnActor<ABomb>(
			BombToSpawn, 
			BombSpawnLocation, 
			GetActorRotation(), 
			SpawnParams
		);

		SpawnedActorRef->BombRangeMultiplier = BombRange;

		BombsSpawned.Emplace(SpawnedActorRef);

		BombCapacity--;
	}
	else
	{
		for (int BombIndex = 0; BombIndex < BombsSpawned.Num(); BombIndex++)
		{
			if (BombsSpawned[BombIndex]->TimeSinceSpawned > BombsSpawned[BombIndex]->ExplosionDelay)
			{
				UE_LOG(LogTemp, Warning, TEXT("Removing: %s"), *BombsSpawned[BombIndex]->GetName());
				BombsSpawned.RemoveAt(BombIndex, 1, true);
				BombIndex--;

				BombCapacity++;
				SpawnBomb();
			}
		}
		
	}
	
}