// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "DrawDebugHelpers.h"
#include "Math/Color.h"
#include "UObject/ConstructorHelpers.h"
#include "Bomberman.h"

// Sets default values

ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	TimeSinceSpawned = GetWorld()->DeltaTimeSeconds;

	SetActorEnableCollision(false);
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TArray<AActor*> OverlappingActors;
	// GetOverlappingActors(OverlappingActors, TSubclassOf<ABomberman>());
	// if (OverlappingActors.Num() == 0)
	// {
	// 	SetActorEnableCollision(true);
	// }
	

	if (TimeSinceSpawned < ExplosionDelay)
	{
		TimeSinceSpawned += GetWorld()->DeltaTimeSeconds;
		
		float BombRange = BombRangeBase + (BombRangeMultiplier - 1.f) * 100.f;

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			FVector(GetActorLocation().X + BombRange, GetActorLocation().Y, GetActorLocation().Z),
			FColor::Green,
			false
		);

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			FVector(GetActorLocation().X - BombRange, GetActorLocation().Y, GetActorLocation().Z),
			FColor::Green,
			false
		);

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			FVector(GetActorLocation().X, GetActorLocation().Y + BombRange, GetActorLocation().Z),
			FColor::Green,
			false
		);

		DrawDebugLine(
			GetWorld(),
			GetActorLocation(),
			FVector(GetActorLocation().X, GetActorLocation().Y - BombRange, GetActorLocation().Z),
			FColor::Green,
			false
		);
	}
	
	else
	{
		Destroy();
	}
	
}