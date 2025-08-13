// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/PACharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "State/PAStateComponent.h"
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

	/** Anim Notify */
	UFUNCTION()
	void AnimNotify_ResetMovementInputEnabled(UAnimNotify* Notify);

	UFUNCTION()
	void AnimNotify_ResetState(UAnimNotify* Notify);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	APACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reference")
	UPAStateComponent* StateComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	UPAAttributeComponent* AttributeComponent;

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
