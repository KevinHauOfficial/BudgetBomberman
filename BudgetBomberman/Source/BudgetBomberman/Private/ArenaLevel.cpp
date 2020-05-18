// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaLevel.h"
#include "BreakableBlocks.h"
#include "Engine/World.h"

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