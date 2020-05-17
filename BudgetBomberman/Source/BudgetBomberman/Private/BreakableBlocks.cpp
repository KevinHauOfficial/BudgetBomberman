// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableBlocks.h"
#include "Engine/World.h"

#include "PowerUp.h"

// Sets default values
ABreakableBlocks::ABreakableBlocks()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up loot table
	LootTable.Emplace("Nothing") = 70;
	LootTable.Emplace("MovementSpeedUp") = 6;
	LootTable.Emplace("BombCapacityUp") = 11;
	LootTable.Emplace("MaxBombCapacityUp") = 1;
	LootTable.Emplace("BombRangeUp") = 11;
	LootTable.Emplace("MaxBombRangeUp") = 1;
}

// Called when the game starts or when spawned
void ABreakableBlocks::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableBlocks::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableBlocks::DropPowerUp()
{
	int32 TotalSum = CalculateSum();
	int32 RandomNumber = rand() % TotalSum;

	TArray<FString> Keys;
	LootTable.GetKeys(Keys);

	for (FString Key : Keys)
	{
		if (RandomNumber < LootTable[Key])
		{
			// TODO drop LootTable[Key] powerup
			UE_LOG(LogTemp, Warning, TEXT("%s dropped a %s!"), *GetName(), *Key);

			if (Key != "Nothing")
			{
				FActorSpawnParameters SpawnParams;
				APowerUp* SpawnedActorRef = GetWorld()->SpawnActor<APowerUp>(
					PowerUpToSpawn, 
					GetActorLocation(), 
					GetActorRotation(), 
					SpawnParams
				);
				SpawnedActorRef->PowerUp = Key;
			}

			break;
		}
		else 
		{
			RandomNumber -= LootTable[Key];
		}
	}
	
}

int32 ABreakableBlocks::CalculateSum()
{
	int32 Sum = 0;

	TArray<FString> Keys;
	LootTable.GetKeys(Keys);

	for (FString Key : Keys)
	{
		Sum += LootTable[Key];
	}

	return Sum;
}

void ABreakableBlocks::Destroy()
{
	DropPowerUp();
	Super::Destroy();
}