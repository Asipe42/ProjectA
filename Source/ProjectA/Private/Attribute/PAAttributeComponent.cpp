// Copyright Modin


#include "Attribute/PAAttributeComponent.h"

#include "PADefine.h"

UPAAttributeComponent::UPAAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPAAttributeComponent::DecreaseStamina(const float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0, MaxStamina);
	OnAttributeChange.Broadcast(EAttributeType::Stamina, GetStaminaRatio());
}

void UPAAttributeComponent::RegenerateStamina(bool bEnable, float Delay)
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
					OnAttributeChange.Broadcast(EAttributeType::Stamina, GetStaminaRatio());
					if (CurrentStamina >= MaxStamina)
					{
						GetWorld()->GetTimerManager().ClearTimer(RegenerationTimerHandle);
					}
				},
				0.1f,
				true,
				Delay
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
