// Copyright Modin


#include "Character/Player/PAPlayerAnimInstance.h"

#include "GameFramework/Character.h"

void UPAPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	InitializeReference();
}

void UPAPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateMovementProperties();
}

void UPAPlayerAnimInstance::InitializeReference()
{
	/*
	 * 참조 초기화
	 *	- Proc 1. Character 초기화
	 *	- Proc 2. MovementComponent 초기화
	 */

	// Proc 1
	Character = Cast<ACharacter>(GetOwningActor());

	// Proc 2
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UPAPlayerAnimInstance::UpdateMovementProperties()
{
	if (Character == nullptr || MovementComponent == nullptr)
		return;

	/*
	 * 이동 속성 갱신
	 * 	- Proc 1. Velocity 갱신
	 * 	- Proc 2. GroundSpeed 갱신
	 * 	- Proc 3. bShouldMove 갱신
	 * 	- Proc 4. bIsFalling 갱신
	 */
	
	// Proc 1
	Velocity = MovementComponent->Velocity;

	// Proc 2
	GroundSpeed = Velocity.Size2D();

	// Proc 3
	constexpr float MovementSpeedThreshold = 3.f;
	bShouldMove = GroundSpeed > MovementSpeedThreshold && !MovementComponent->GetCurrentAcceleration().IsNearlyZero();

	// Proc 4
	bIsFalling = MovementComponent->IsFalling();
}
