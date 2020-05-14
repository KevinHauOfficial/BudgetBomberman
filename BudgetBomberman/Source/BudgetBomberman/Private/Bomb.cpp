// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
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
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, TSubclassOf<ABomberman>());
	
	
	TimeSinceSpawned += GetWorld()->DeltaTimeSeconds;

	if (TimeSinceSpawned < ExplosionDelay)
	{
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
		float BombRange = BombRangeBase + (BombRangeMultiplier - 1.f) * 100.f;

		TArray<AActor*> ActorsToIgnore;
		FHitResult OutHit;

		this->SetActorEnableCollision(false);

		// Check West
		UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X, GetActorLocation().Y - BombRange, GetActorLocation().Z),
			FVector(40.f, 0.f, 40.f),
			FRotator(0.f, 0.f, 0.f),
			ETraceTypeQuery::TraceTypeQuery_MAX,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true,
			FLinearColor(FColor::Red),
			FLinearColor(FColor::Green)
		);

		// Check North
		UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X + BombRange, GetActorLocation().Y, GetActorLocation().Z),
			FVector(0.f, 40.f, 40.f),
			FRotator(0.f, 0.f, 0.f),
			ETraceTypeQuery::TraceTypeQuery_MAX,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true,
			FLinearColor(FColor::Red),
			FLinearColor(FColor::Green)
		);

		// Check East
		UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X, GetActorLocation().Y + BombRange, GetActorLocation().Z),
			FVector(40.f, 0.f, 40.f),
			FRotator(0.f, 0.f, 0.f),
			ETraceTypeQuery::TraceTypeQuery_MAX,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true,
			FLinearColor(FColor::Red),
			FLinearColor(FColor::Green)
		);

		// Check South
		UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X - BombRange, GetActorLocation().Y, GetActorLocation().Z),
			FVector(0.f, 40.f, 40.f),
			FRotator(0.f, 0.f, 0.f),
			ETraceTypeQuery::TraceTypeQuery_MAX,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHit,
			true,
			FLinearColor(FColor::Red),
			FLinearColor(FColor::Green)
		);

		Destroy();
	}
	
}