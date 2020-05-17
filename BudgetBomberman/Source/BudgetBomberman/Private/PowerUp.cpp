// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Bomberman.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent = FindComponentByClass<UBoxComponent>();
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFUNCTION( )
void APowerUp::BeginOverlap (UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult &SweepResult)
{
	ABomberman* Player = Cast<ABomberman>(OtherActor);

	if (PowerUp == "MovementSpeedUp")
	{
		Player->MovementSpeed += 0.5f;
	}
	else if (PowerUp == "BombCapacityUp")
	{
		if (Player->BombCapacity < Player->MaxBombCapacity)
		{
			Player->BombCapacity++;
		}
	}
	else if (PowerUp == "BombRangeUp")
	{
		if (Player->BombRange < Player->MaxBombRange)
		{
			Player->BombRange++;
		}
	}
	else if (PowerUp == "MaxBombRangeUp")
	{
		Player->MaxBombRange++;
	}
	
	Destroy();	
}
