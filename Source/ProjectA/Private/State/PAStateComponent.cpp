// Copyright Modin


#include "State/PAStateComponent.h"

UPAStateComponent::UPAStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UPAStateComponent::IsCurrentStateIn(const FGameplayTagContainer& TagsToCheck) const
{
	return TagsToCheck.HasTagExact(CurrentState);
}

void UPAStateComponent::SetMovementEnabledWithDelay(const bool bEnable, const float Delay)
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	if (World->GetTimerManager().IsTimerActive(MovementEnableTimerHandle))
	{
		World->GetTimerManager().ClearTimer(MovementEnableTimerHandle);
	}
	
	if (bEnable)
	{
		World->GetTimerManager().SetTimer
		(
			MovementEnableTimerHandle,
			[this]() { SetMovementEnabled(true); },
			Delay,
			false
		);
	}
	else
	{
		SetMovementEnabled(false);
	}
}
