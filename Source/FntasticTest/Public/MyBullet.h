// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyBullet.generated.h"

UCLASS()
class FNTASTICTEST_API AMyBullet : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionBullet;

	UFUNCTION()
	void onHitBullet();
};
