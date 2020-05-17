// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bomberman.generated.h"

class ABomb;

UCLASS()
class BUDGETBOMBERMAN_API ABomberman : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABomberman();

	// Stats
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	bool bAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MovementSpeed = 1.f; // Can be increased with Speed powerup (drops by X levels on death)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BombCapacity = 1.f; // Can be increased with Capacity powerup (until max capacity)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxBombCapacity = 10.f; // Can be increased with Max Capacity powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BombRange = 2.f; // Can be increased by Range powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxBombRange = 10.f;

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FVector CurrentVelocity;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	// Bomb spawning
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<ABomb> BombToSpawn;

	UFUNCTION()
	void SpawnBomb();
	UFUNCTION()
	void KillPlayer();

private:
	FVector GetBombSpawnLocation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
