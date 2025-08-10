// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PAPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UPAPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bShouldMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsFalling;

private:
	void InitializeReference();
	void UpdateMovementProperties();
};
