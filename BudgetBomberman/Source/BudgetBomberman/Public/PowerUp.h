// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

class UBoxComponent;

UCLASS()
class BUDGETBOMBERMAN_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties")
	FString PowerUp = "";

	// Overlap
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent = nullptr;

	UFUNCTION()
	void OnBeginOverlap(
		class UPrimitiveComponent* Comp, 
		class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
