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

	if (TimeSinceSpawned >= ExplosionDelay)
	{
		// Determines bomb range
		float BombRange = BombRangeBase + (BombRangeMultiplier - 1.f) * 100.f;
		// Stores the actor hit
		FHitResult OutHit;

		// Prevents bomb from hitting itself
		this->SetActorEnableCollision(false);
		// Increases capacity; performed early in case player dies and reference cannot be accessed
		OwnedBy->BombCapacity++;
		
		// Check north explosion
		if (CheckExplosionDirection(OutHit, BombRange, 0.f))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s hit north!"), *OutHit.GetActor()->GetName());
			ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
			if (HitPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is a player!"), *HitPlayer->GetName());
				HitPlayer->SpawnBomb();
			}
		}
		//Check south explosion
		if (CheckExplosionDirection(OutHit, -BombRange, 0.f))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s hit south!"), *OutHit.GetActor()->GetName());
			ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
			if (HitPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is a player!"), *HitPlayer->GetName());
				HitPlayer->SpawnBomb();
			}
		}
		//Check east explosion
		if (CheckExplosionDirection(OutHit, 0.f, BombRange))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s hit east!"), *OutHit.GetActor()->GetName());
			ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
			if (HitPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is a player!"), *HitPlayer->GetName());
				HitPlayer->SpawnBomb();
			}
		}
		// Check west explosion
		if (CheckExplosionDirection(OutHit, 0.f, -BombRange))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s hit west!"), *OutHit.GetActor()->GetName());
			ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
			if (HitPlayer)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s is a player!"), *HitPlayer->GetName());
				HitPlayer->SpawnBomb();
			}
		}

		Destroy();
	}
}

bool ABomb::CheckExplosionDirection(FHitResult &OutHit, float RangeX, float RangeY)
{
	TArray<AActor*> ActorsToIgnore;
	
	// North & South
	if (RangeX != 0 && RangeY == 0)
	{
		return UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X + RangeX, GetActorLocation().Y, GetActorLocation().Z),
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
	}
	// East & West
	if (RangeX == 0 && RangeY != 0)
	{
		return UKismetSystemLibrary::BoxTraceSingle(
			GetWorld(), 
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FVector(GetActorLocation().X, GetActorLocation().Y + RangeY, GetActorLocation().Z),
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
	}

	return false;
}