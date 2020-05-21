// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaLevel.h"
#include "BreakableBlocks.h"
#include "Engine/World.h"

AArenaLevel::AArenaLevel()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AArenaLevel::BeginPlay()
{
	Super::BeginPlay();

    GenerateMap();
}

void AArenaLevel::GenerateMap() const
{
    for (int Row = -900; Row <= 900; Row += 100)
    {
        for (int Column = -900; Column <= 900; Column += 100)
        {
            // Spawn if location isn't the three blocks at each corner
            if ( !((abs(Row) == 900 && (abs(Column) == 900 || abs(Column) == 800)) ||
                (abs(Row) == 800 && abs(Column) == 900)))
            {
                SpawnBreakableBlocks(Row, Column);
            }
        }
    }
}

void AArenaLevel::SpawnBreakableBlocks(int Row, int Column) const
{
    float RandomNumber = rand() % 100;

    if (RandomNumber < SpawnChance)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
        ABreakableBlocks* SpawnedActorRef = GetWorld()->SpawnActor<ABreakableBlocks>(
            BreakableBlock, 
            FVector(Row, Column, 60.f), 
            FRotator(0.f, 0.f, 0.f), 
            SpawnParams
        );
    }
}

void AArenaLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Timer > 0)
    {
        Timer = FMath::Clamp(Timer - DeltaTime, 0.f, RoundTime);
        UE_LOG(LogTemp, Warning, TEXT("Time remaining in seconds: %f"), Timer);
    }
    
}