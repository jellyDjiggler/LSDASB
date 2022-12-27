// Copyright Epic Games, Inc. All Rights Reserved.

#include "LSDASBGameMode.h"
#include "LSDASBCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALSDASBGameMode::ALSDASBGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
