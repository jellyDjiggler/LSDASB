// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LSDASBCharacter.h"
#include "LSDASBGameMode.generated.h"

UCLASS(minimalapi)
class ALSDASBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALSDASBGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ALSDASBCharacter* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ALSDASBCharacter* player2;
};
