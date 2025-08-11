// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PAStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPAStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPAStateComponent();

	/** State Method */
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; }
	FORCEINLINE void SetState(const FGameplayTag& NewState) { CurrentState = NewState; }
	FORCEINLINE void ClearState() { CurrentState = FGameplayTag::EmptyTag; }
	FORCEINLINE bool IsCurrentState(const FGameplayTag& StateToCheck) const { return CurrentState == StateToCheck; }

	/** Movement Method */
	FORCEINLINE bool IsMovementEnabled() const { return bIsMovementEnabled; }
	FORCEINLINE void SetMovementEnabled(const bool bEnable) { bIsMovementEnabled = bEnable; }
	void SetMovementEnabledWithDelay(const bool bEnable, const float Delay);

protected:
	/** State Field */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTag CurrentState;
	
	/** Movement Field */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsMovementEnabled = true;

	FTimerHandle MovementEnableTimerHandle;
};
