// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"
#include "UObject/ConstructorHelpers.h"

#include "Bomberman.h"
#include "BreakableBlocks.h"

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

	// Accumulates time since bomb has been spawned
	TimeSinceSpawned += GetWorld()->DeltaTimeSeconds;

	if (TimeSinceSpawned >= ExplosionDelay)
	{
		// Prevents bomb from hitting itself
		this->SetActorEnableCollision(false);
		// Increases capacity; performed early in case player dies and reference cannot be accessed
		OwnedBy->BombCapacity++;

		// Explodes and makes appropriate calls in objects that were hit by the bomb's explosion
		Explosion();
		// Destroys self
		Destroy();
	}
}

void ABomb::Explosion()
{
	float BombRange = BombRangeBase + (BombRangeMultiplier - 1.f) * 100.f;
	FHitResult OutHit;

	for (int32 Counter = 1; Counter < 5; Counter++)
	{
		// North and South
		if (Counter < 3)
		{
			if (CheckExplosionDirection(OutHit, pow(-1, Counter) * BombRange, 0.f))
			{
				ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
				if (HitPlayer)
				{
					HitPlayer->KillPlayer();
					// TODO reduce number of players remaining
				}
				else
				{
					// TODO destructible object
					ABreakableBlocks* HitBreakableBlock = Cast<ABreakableBlocks>(OutHit.GetActor());
					if (HitBreakableBlock)
					{
						HitBreakableBlock->Destroy();
					}
				}
				
			}
		}
		// East and West
		else
		{
			if (CheckExplosionDirection(OutHit, 0.f, pow(-1, Counter) * BombRange))
			{
				ABomberman* HitPlayer = Cast<ABomberman>(OutHit.GetActor());
				if (HitPlayer)
				{
					HitPlayer->KillPlayer();
					// TODO reduce number of players remaining
				}
				else
				{
					// TODO destructible object
					ABreakableBlocks* HitBreakableBlock = Cast<ABreakableBlocks>(OutHit.GetActor());
					if (HitBreakableBlock)
					{
						HitBreakableBlock->Destroy();
					}
				}
			}
		}
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