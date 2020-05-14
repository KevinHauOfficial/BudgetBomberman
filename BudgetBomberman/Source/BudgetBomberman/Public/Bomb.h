// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

class ABomberman;

UCLASS()
class BUDGETBOMBERMAN_API ABomb : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABomb();

	// Properties
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	ABomberman* OwnedBy = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	float TimeSinceSpawned = 0.f;
	UPROPERTY(EditAnywhere, Category = "Properties")
	float ExplosionDelay = 3.f;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	float BombRangeBase = 150.f;
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	float BombRangeMultiplier = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool CheckExplosionDirection(FHitResult &OutHit, float RangeX, float RangeY);

};