// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxType : uint8
{
	HB_PROXIMITY		UMETA(DisplayName = "Proximity"),
	HB_STRIKE			UMETA(DisplayName = "Strike"),
	HB_HURTBOX			UMETA(DisplayName = "Hurtbox")
};

UCLASS()
class LSDASB_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	UFUNCTION(BlueprintCallable)
		void TriggerVisualizeHitbox();

	UFUNCTION(BlueprintImplementableEvent)
		void VisualizeHitbox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitstunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float blockstunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxType hitboxType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
