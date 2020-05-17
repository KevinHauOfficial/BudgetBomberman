// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BreakableBlocks.generated.h"

UCLASS()
class BUDGETBOMBERMAN_API ABreakableBlocks : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableBlocks();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
	TMap<FString, int32> LootTable;

	void Destroy();
	void DropPowerUp();

private:
	int32 CalculateSum();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
