// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FNTASTICTEST_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
    void BeginPlay() override;

private:

    class UNavigationSystemV1* NavArea;

    FNavLocation RandomLocation;
    FVector SpawnPoint;

public:

    UFUNCTION()
    void RandomPatrol();

    UFUNCTION()
    void GoToHearPlayerLocation(FVector playerPosistion);

    void setSpawnPoint(FVector Loc);
};
