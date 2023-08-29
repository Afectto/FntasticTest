// Copyright Epic Games, Inc. All Rights Reserved.

#include "FntasticTestGameMode.h"
#include "FntasticTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFntasticTestGameMode::AFntasticTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
