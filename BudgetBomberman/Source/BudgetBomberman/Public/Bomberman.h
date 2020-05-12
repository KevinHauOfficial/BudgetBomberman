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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 1.f; // Can be increased with Health powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxHealth = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MovementSpeed = 2.f; // Can be increased with Speed powerup (drops by X levels on death)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BombCapacity = 1.f; // Can be increased with Capacity powerup (until max capacity)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxBombCapacity = 10.f; // Can be increased with Max Capacity powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float BombRange = 1.f; // Can be increased by Range powerup
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float MaxBombRange = 10.f;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	FVector CurrentVelocity;


	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> BombToSpawn;
	UFUNCTION()
	void SpawnBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
