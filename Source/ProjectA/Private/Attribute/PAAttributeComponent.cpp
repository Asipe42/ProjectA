// Copyright Modin


#include "Attribute/PAAttributeComponent.h"

UPAAttributeComponent::UPAAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPAAttributeComponent::DecreaseStamina(const float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0, MaxStamina);
}

void UPAAttributeComponent::RegenerateStamina(bool bEnable)
{
	if (bEnable)
	{
		if (CurrentStamina >= MaxStamina)
			return;
		
		if (!GetWorld()->GetTimerManager().IsTimerActive(RegenerationTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer
			(
				RegenerationTimerHandle,
				[this]
				{
					CurrentStamina = FMath::Clamp(CurrentStamina + RegenerateStaminaRate, 0.f, MaxStamina);
					if (CurrentStamina >= MaxStamina)
					{
						GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
					}
				},
				0.1f,
				true
			);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
	}
}

bool UPAAttributeComponent::HasEnoughStamina(const float Amount) const
{
	return CurrentStamina >= Amount;
}
