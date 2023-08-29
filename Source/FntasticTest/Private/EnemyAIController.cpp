// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"


void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    //NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    SpawnPoint = FVector();

}

void AEnemyAIController::RandomPatrol()
{
    if (NavArea)
    {
        //NavArea->GetRandomReachablePointInRadius(SpawnPoint, 2000.0f, RandomLocation);
        GEngine->AddOnScreenDebugMessage(70, 4, FColor::Green, "Enemy patrolling");
        MoveToLocation(RandomLocation.Location);
    }
}

void AEnemyAIController::GoToHearPlayerLocation(FVector playerPosistion)
{
    if (NavArea)
    {
        //NavArea->GetRandomReachablePointInRadius(playerPosistion, 1000.0f, RandomLocation);
        GEngine->AddOnScreenDebugMessage(60, 4, FColor::Yellow, "Enemy hear you");
        MoveToLocation(RandomLocation.Location);
    }
}

void AEnemyAIController::setSpawnPoint(FVector Loc)
{
    SpawnPoint = Loc;
}
