// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"

#include "Bomberman.h"
#include "BreakableBlocks.h"
#include "PowerUp.h"

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

	// Find the bomb mesh and set it to a dynamic mesh to change colour
	UStaticMeshComponent* Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);
}

// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Accumulates time since bomb has been spawned
	TimeSinceSpawned += GetWorld()->DeltaTimeSeconds;

	// Changes the color of the bomb when its closer to detonating
	float Blend = FMath::Clamp<float>(TimeSinceSpawned / ExplosionDelay + 0.3, 0.f, 1.f);
	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), Blend);

	if (TimeSinceSpawned >= ExplosionDelay)
	{
		// Prevents bomb from hitting itself
		this->SetActorEnableCollision(false);
		// Replenishes capacity; performed early in case player dies and reference cannot be accessed
		OwnedBy->BombCapacity++;

		// Explodes and makes appropriate calls in objects that were hit by the bomb's explosion
		Explosion();
		Destroy();
	}
}

void ABomb::Explosion() const
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
				AActor* HitActor = OutHit.GetActor();
				ExplosionDestroy(HitActor);
			}
		}
		// East and West
		else
		{
			if (CheckExplosionDirection(OutHit, 0.f, pow(-1, Counter) * BombRange))
			{
				AActor* HitActor = OutHit.GetActor();
				ExplosionDestroy(HitActor);
			}
		}
	}
}

void ABomb::ExplosionDestroy(AActor* HitActor) const
{
	if (Cast<ABomberman>(HitActor))
	{
		ABomberman* HitPlayer = Cast<ABomberman>(HitActor);
		HitPlayer->KillPlayer();
	}
	else if (Cast<ABreakableBlocks>(HitActor))
	{
		ABreakableBlocks* HitBreakableBlock = Cast<ABreakableBlocks>(HitActor);
		HitBreakableBlock->Destroy();
	}
	else if (Cast<APowerUp>(HitActor))
	{
		APowerUp* HitPowerUp = Cast<APowerUp>(HitActor);
		HitPowerUp->Destroy();
	}
	else if (Cast<ABomb>(HitActor))
	{
		ABomb* HitBomb  = Cast<ABomb>(HitActor);
		if (HitBomb->ExplosionDelay - HitBomb->TimeSinceSpawned > 0.5f)
		{
			HitBomb->TimeSinceSpawned = HitBomb->ExplosionDelay - 0.5f;
		}
	}
}

bool ABomb::CheckExplosionDirection(FHitResult &OutHit, float RangeX, float RangeY) const
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