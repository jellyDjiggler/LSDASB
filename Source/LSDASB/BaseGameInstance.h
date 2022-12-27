// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_LloydMillsIII				UMETA(DisplayName = "Lloyd Mills III"),
	VE_GavinTaylor					UMETA(DisplayName = "Gavin Taylor"),
	VE_VonnHaring					UMETA(DisplayName = "Vonn Haring"),
	VE_LloydBlack					UMETA(DisplayName = "Lloyd Black"),
	VE_AikawaIshiki					UMETA(DisplayName = "Aikawa Ishiki"),
	VE_SilverYamato					UMETA(DisplayName = "Silver Yamato"),
	VE_LeonMazura					UMETA(DisplayName = "Leon Mazura"),
	VE_Grey							UMETA(DisplayName = "Grey"),
	VE_RenzoFurea					UMETA(DisplayName = "Renzo Furea")
};

UCLASS()
class LSDASB_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ECharacterClass characterClass;
};
