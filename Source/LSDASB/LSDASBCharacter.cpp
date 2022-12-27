// Copyright Epic Games, Inc. All Rights Reserved.

#include "LSDASBCharacter.h"
#include "LSDASBGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ALSDASBCharacter::ALSDASBCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/*
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	*/

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	otherPlayer = nullptr;
	hurtbox = nullptr;
	characterState = ECharacterState::VE_Default;
	transform = FTransform();
	scale = FVector(0.0f, 0.0f, 0.0f);

	playerHealth = 1.00f;
	maxDistanceApart = 800.0f;
	stunTime = 0.0f;

	wasLightAttackUsed = false;
	wasMediumAttackUsed = false;
	wasHeavyAttackUsed = false;
	wasSpecialAttackUsed = false;
	isFlipped = false;
	hasLandedHit = false;
	canMove = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALSDASBCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (auto gameMode = Cast<ALSDASBGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player 1 controls bound."));

			// set up gameplay key bindings
			PlayerInputComponent->BindAction("JumpP1", IE_Pressed, this, &ALSDASBCharacter::Jump);
			PlayerInputComponent->BindAction("JumpP1", IE_Released, this, &ALSDASBCharacter::StopJumping);
			PlayerInputComponent->BindAction("CrouchP1", IE_Pressed, this, &ALSDASBCharacter::StartCrouching);
			PlayerInputComponent->BindAction("CrouchP1", IE_Released, this, &ALSDASBCharacter::StopCrouching);
			PlayerInputComponent->BindAxis("MoveRightP1", this, &ALSDASBCharacter::MoveRight);

			PlayerInputComponent->BindAction("LightAttackP1", IE_Pressed, this, &ALSDASBCharacter::StartLightAttack);
			PlayerInputComponent->BindAction("MediumAttackP1", IE_Pressed, this, &ALSDASBCharacter::StartMediumAttack);
			PlayerInputComponent->BindAction("HeavyAttackP1", IE_Pressed, this, &ALSDASBCharacter::StartHeavyAttack);
			PlayerInputComponent->BindAction("SpecialAttackP1", IE_Pressed, this, &ALSDASBCharacter::StartSpecialAttack);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ALSDASBCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ALSDASBCharacter::TouchStopped);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player 2 controls bound."));

			// set up gameplay key bindings
			PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ALSDASBCharacter::Jump);
			PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ALSDASBCharacter::StopJumping);
			PlayerInputComponent->BindAction("CrouchP2", IE_Pressed, this, &ALSDASBCharacter::StartCrouching);
			PlayerInputComponent->BindAction("CrouchP2", IE_Released, this, &ALSDASBCharacter::StopCrouching);
			PlayerInputComponent->BindAxis("MoveRightP2", this, &ALSDASBCharacter::MoveRight);

			PlayerInputComponent->BindAction("LightAttackP2", IE_Pressed, this, &ALSDASBCharacter::StartLightAttack);
			PlayerInputComponent->BindAction("MediumAttackP2", IE_Pressed, this, &ALSDASBCharacter::StartMediumAttack);
			PlayerInputComponent->BindAction("HeavyAttackP2", IE_Pressed, this, &ALSDASBCharacter::StartHeavyAttack);
			PlayerInputComponent->BindAction("SpecialAttackP2", IE_Pressed, this, &ALSDASBCharacter::StartSpecialAttack);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ALSDASBCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ALSDASBCharacter::TouchStopped);
		}
	}
	
}

void ALSDASBCharacter::Jump()
{
	if (canMove)
	{
		ACharacter::Jump();
		characterState = ECharacterState::VE_Jumping;
	}
}

void ALSDASBCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ALSDASBCharacter::Landed(const FHitResult& Hit)
{
	characterState = ECharacterState::VE_Default;
}

void ALSDASBCharacter::StartCrouching()
{
	characterState = ECharacterState::VE_Crouching;
}

void ALSDASBCharacter::StopCrouching()
{
	characterState = ECharacterState::VE_Default;
}

void ALSDASBCharacter::StartBlocking()
{
	characterState = ECharacterState::VE_Blocking;
}

void ALSDASBCharacter::StopBlocking()
{
	characterState = ECharacterState::VE_Default;
}

void ALSDASBCharacter::MoveRight(float Value)
{
	if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Current direction is: %f"), Value);

		if (characterState != ECharacterState::VE_Jumping)
		{
			if (Value > 0.20f)
			{
				characterState = ECharacterState::VE_MovingRight;
			}
			else if (Value < -0.20f)
			{
				characterState = ECharacterState::VE_MovingLeft;
			}
			else
			{
				characterState = ECharacterState::VE_Default;
			}
		}

		float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

		if (currentDistanceApart >= maxDistanceApart)
		{
			if ((currentDistanceApart + Value < currentDistanceApart && !isFlipped) || (currentDistanceApart - Value < currentDistanceApart && isFlipped))
			{
				AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
			}
		}
		else
		{
			AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
		}
	}
}

void ALSDASBCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ALSDASBCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ALSDASBCharacter::StartLightAttack()
{
	wasLightAttackUsed = true;
	UE_LOG(LogTemp, Warning, TEXT("Using LIGHT Attack"));
}

void ALSDASBCharacter::StartMediumAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Using MEDIUM Attack"));
	wasMediumAttackUsed = true;
}

void ALSDASBCharacter::StartHeavyAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Using HEAVY Attack"));
	wasHeavyAttackUsed = true;
}

void ALSDASBCharacter::StartSpecialAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Using SPECIAL Attack"));
	wasSpecialAttackUsed = true;
}

void ALSDASBCharacter::P2KeyboardLightAttack()
{
	StartLightAttack();
}

void ALSDASBCharacter::P2KeyboardMediumAttack()
{
	StartMediumAttack();
}


void ALSDASBCharacter::P2KeyboardHeavyAttack()
{
	StartHeavyAttack();
}


void ALSDASBCharacter::P2KeyboardSpecialAttack()
{
	StartSpecialAttack();
}

void ALSDASBCharacter::P2KeyboardJump()
{
	Jump();
}

void ALSDASBCharacter::P2KeyboardStopJumping()
{
	StopJumping();
}

void ALSDASBCharacter::P2KeyboardMoveRight(float _value)
{
	MoveRight(_value);
}

void ALSDASBCharacter::CollidedWithProximityHitbox()
{
	if ((characterState == ECharacterState::VE_MovingLeft && isFlipped) || (characterState == ECharacterState::VE_MovingRight && !isFlipped))
	{
		UE_LOG(LogTemp, Warning, TEXT("Blocking."));
		characterState = ECharacterState::VE_Blocking;
	}
}

void ALSDASBCharacter::TakeDamage(float _damageAmount, float _hitstunTime, float _blockstunTime)
{
	if (characterState != ECharacterState::VE_Blocking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Taking %f points of damage."), _damageAmount);
		playerHealth -= _damageAmount;

		stunTime = _hitstunTime;

		if (stunTime > 0.0f)
		{
			characterState = ECharacterState::VE_Stunned;
			BeginStun();
		}


		if (otherPlayer)
		{
			otherPlayer->hasLandedHit = true;
		}
	}
	else
	{
		float reducedDamage = _damageAmount * 0.5f;
		UE_LOG(LogTemp, Warning, TEXT("Taking %f points of REDUCED damage."), reducedDamage);
		playerHealth -= reducedDamage;

		stunTime = _blockstunTime;

		if (stunTime > 0.0f)
		{
			BeginStun();
		}
		else
		{
			characterState = ECharacterState::VE_Default;
		}
	}
	
	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
	}

	if (playerHealth > 1.00f)
	{
		playerHealth = 1.00f;
	}
}

void ALSDASBCharacter::BeginStun()
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ALSDASBCharacter::EndStun, stunTime, false);
}

void ALSDASBCharacter::EndStun()
{
	characterState = ECharacterState::VE_Default;
	canMove = true;
}

void ALSDASBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (characterState != ECharacterState::VE_Jumping)
	{
		if (otherPlayer)
		{
			if (auto characterMovement = GetCharacterMovement())
			{
				if (auto enemyMovement = otherPlayer->GetCharacterMovement())
				{
					if (enemyMovement->GetActorLocation().Y <= characterMovement->GetActorLocation().Y)
					{
						if (isFlipped)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = -1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);
							}
							isFlipped = false;
						}
					}
					else
					{
						if (!isFlipped)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = 1;
								transform.SetScale3D(scale);
								mesh->SetRelativeTransform(transform);
							}
							isFlipped = true;
						}
					}
				}
			}
		}
	}
}