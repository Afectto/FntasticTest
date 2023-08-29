// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "FntasticTest/FntasticTestCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "MyBullet.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300;

	PlayerCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));
	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollisionDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

	CollisionDetectionSeePlayer = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Detection See Player"));
	CollisionDetectionSeePlayer->SetupAttachment(RootComponent);

	PlayerCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Bullet Box"));
	PlayerCollisionBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->setSpawnPoint(GetActorLocation());

	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnAIMoveCompleted);

	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemy::OnPlayerDetectedOverlapBegin);

	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AEnemy::OnPlayerDetectedOverlapEnd);

	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemy::OnPlayerAttackOverlapBegin);

	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AEnemy::OnPlayerAttackOverlapEnd);

	CollisionDetectionSeePlayer->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemy::OnDetectedSeePlayerOverlapBegin);

	CollisionDetectionSeePlayer->OnComponentEndOverlap.AddDynamic(this,
		&AEnemy::OnDetectedSeePlayerOverlapEnd);

	PlayerCollisionBox->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemy::OnDetectedHitEnemyOverlapBegin);

	TArray<AFntasticTestCharacter*> player;
	FindAllActor(player);
	PlayerREF = player[0];

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!isAiEnabled) return;

	if (!PlayerDetected)
	{
		if (!isEnemySeek) EnemyAIController->RandomPatrol();
	}
	else if (PlayerDetected && CanAttackPlayer)
	{
		StopSeekingPlayer();
	}
	else
	{
		if (PlayerREF && PlayerREF->isSprint())
		{
			SeekHearPlayer();
		}
		else
		{
			if (!isEnemySeek)
			{
				EnemyAIController->RandomPatrol();
			}
		}
	}
}

void AEnemy::MoveToPlayer()
{
	if (!isAiEnabled || !PlayerREF) return;
	EnemyAIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
}

void AEnemy::SeekPlayer()
{
	if (!isAiEnabled) return;
	MoveToPlayer();
	isEnemySeek = true;
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
		&AEnemy::SeekPlayer, 0.25f, true);
}

void AEnemy::SeekHearPlayer()
{
	if (!isAiEnabled || !PlayerREF || isEnemySeek) return;
	if (PlayerREF->isSprint())
	{
		EnemyAIController->GoToHearPlayerLocation(PlayerREF->GetActorLocation());
	}
	GetWorld()->GetTimerManager().SetTimer(SeekHearPlayerTimerHandle, this,
		&AEnemy::SeekHearPlayer, 0.25f, true);
}

void AEnemy::StopSeekingPlayer()
{
	isEnemySeek = false;
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AEnemy::StopSeekingHearPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekHearPlayerTimerHandle);
}

void AEnemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isAiEnabled) return;

	if (PlayerREF)
	{

		PlayerDetected = true;
		SeekHearPlayer();
	}
}

void AEnemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!isAiEnabled) return;

	if (PlayerREF)
	{
		PlayerDetected = false;
		StopSeekingHearPlayer();
	}
}

void AEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isAiEnabled) return;

	if (PlayerREF && isAiEnabled)
	{
		CanAttackPlayer = true;
	}
}

void AEnemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!isAiEnabled) return;

	if (PlayerREF && isAiEnabled)
	{
		CanAttackPlayer = false;

		SeekPlayer();
	}
}


void AEnemy::OnDetectedSeePlayerOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isAiEnabled) return;

	if (PlayerREF)
	{
		GEngine->AddOnScreenDebugMessage(50, 4, FColor::Red, "Enemy see you");
		PlayerDetected = true;
		SeekPlayer();
	}
}

void AEnemy::OnDetectedSeePlayerOverlapEnd(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!isAiEnabled) return;

	if (PlayerREF)
	{
		StopSeekingPlayer();
	}
}


void AEnemy::StartAI()
{
	isAiEnabled = true;
	EnemyAIController->RandomPatrol();

	FHitResult empty;
	if (PlayerCollisionDetection->IsOverlappingActor(PlayerREF)) OnPlayerDetectedOverlapBegin(NULL, PlayerREF, NULL, NULL, NULL, empty);
	if (PlayerAttackCollisionDetection->IsOverlappingActor(PlayerREF)) OnPlayerAttackOverlapBegin(NULL, PlayerREF, NULL, NULL, NULL, empty);
	if (CollisionDetectionSeePlayer->IsOverlappingActor(PlayerREF)) OnDetectedSeePlayerOverlapBegin(NULL, PlayerREF, NULL, NULL, NULL, empty);


	HitPoints = 10;
}

void AEnemy::StopAI()
{
	EnemyAIController->MoveToLocation(GetActorLocation());
	isAiEnabled = false;
	StopSeekingPlayer();
	StopSeekingHearPlayer();
}

template<typename T>
void AEnemy::FindAllActor(TArray<T*>& Out)
{
	for (TActorIterator<T> It(GetWorld()); It; ++It)
	{
		Out.Add(*It);
	}
}


void AEnemy::OnDetectedHitEnemyOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isAiEnabled) return;
	AMyBullet* Bullet = Cast<AMyBullet>(OtherActor);

	if (Bullet)
	{
		HitPoints--;

		if (HitPoints <= 0)
		{
			GEngine->AddOnScreenDebugMessage(708000, 4, FColor::Purple, "Enemy Died");
			StopAI();
		}

		SeekPlayer();
		Bullet->onHitBullet();

		GEngine->AddOnScreenDebugMessage(7088, 4, FColor::Red, "You Hit Enemy");
	}
}
