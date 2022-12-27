// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LSDASBCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default			UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingRight		UMETA(DisplayName = "MOVING_RIGHT"),
	VE_RunningRight		UMETA(DisplayName = "RUNNING_RIGHT"),
	VE_MovingLeft		UMETA(DisplayName = "MOVING_LEFT"),
	VE_RunningLeft		UMETA(DisplayName = "RUNNING_LEFT"),
	VE_Jumping			UMETA(DisplayName = "JUMPING"),
	VE_Stunned			UMETA(DisplayName = "STUNNED"),
	VE_Blocking			UMETA(DisplayName = "BLOCKING"),
	VE_Crouching		UMETA(DisplayName = "CROUCHING")
};

UCLASS(config=Game)
class ALSDASBCharacter : public ACharacter
{
	GENERATED_BODY()

	/*
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	// Camera boom positioning the camera beside the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	*/

	void StartLightAttack();
	void StartMediumAttack();
	void StartHeavyAttack();
	void StartSpecialAttack();

	// FOR KEYBOARD ONLY MODE TO USE PLAYER 2
	UFUNCTION(BlueprintCallable)
		void P2KeyboardLightAttack();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardMediumAttack();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardHeavyAttack();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardSpecialAttack();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardJump();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardStopJumping();

	UFUNCTION(BlueprintCallable)
		void P2KeyboardMoveRight(float _value);

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void Tick(float DeltaTime) override;

	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
		void StartCrouching();

	UFUNCTION(BlueprintCallable)
		void StopCrouching();

	UFUNCTION(BlueprintCallable)
		void StartBlocking();

	UFUNCTION(BlueprintCallable)
		void StopBlocking();

	UFUNCTION(BlueprintCallable)
		void CollidedWithProximityHitbox();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount, float _hitstunTime, float _blockstunTime);

	void BeginStun();

	void EndStun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ALSDASBCharacter* otherPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		AActor* hurtbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		ECharacterState characterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FTransform transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		FVector scale;

	FTimerHandle stunTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float playerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float maxDistanceApart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float stunTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasLightAttackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasMediumAttackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasHeavyAttackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasSpecialAttackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		bool isFlipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
		bool hasLandedHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool canMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
		bool isDeviceForMultiplePlayers;

public:
	ALSDASBCharacter();

	/*
	// Returns SideViewCameraComponent subobject
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	*/
};
