// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Bomberman.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->BodyInstance.SetCollisionProfileName("BlockAll");
	BoxComponent->SetMobility(EComponentMobility::Static);
	BoxComponent->CastShadow = false;
	BoxComponent->SetBoxExtent(FVector(30.f, 30.f, 10.f));
	BoxComponent->SetupAttachment(RootComponent);

	RootComponent = BoxComponent;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	if (BoxComponent)
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnBeginOverlap);
	}
	
}

void APowerUp::OnBeginOverlap(
	class UPrimitiveComponent* Comp, 
	class AActor* OtherActor, 
	class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	ABomberman* Player = Cast<ABomberman>(OtherActor);
	if (Player)
	{
		SetActorEnableCollision(false);

		if (PowerUp == "MovementSpeedUp")
		{
			Player->MovementSpeed += 0.5;
		}
		else if (PowerUp == "BombCapacityUp" && Player->BombCapacity < Player->MaxBombRange)
		{
			Player->BombCapacity++;
		}
		else if (PowerUp == "BombRangeUp" && Player->BombRange < Player->MaxBombRange)
		{
			Player->BombRange++;
		}
		else if (PowerUp == "MaxBombRangeUp")
		{
			Player->MaxBombRange++;
		}

		Destroy();
	}
	
}