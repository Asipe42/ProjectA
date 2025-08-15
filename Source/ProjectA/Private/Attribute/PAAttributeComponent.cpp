// Copyright Modin


#include "Attribute/PAAttributeComponent.h"

#include "PADefine.h"

UPAAttributeComponent::UPAAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPAAttributeComponent::ModifyStamina(const float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0, MaxStamina);
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

void UPAAttributeComponent::ModifyHealth(float Amount)
{
	if (FMath::IsNearlyZero(Amount) || CurrentHealth <= 0.f)
		return;

	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);

	if (OnAttributeChange.IsBound())
	{
		OnAttributeChange.Broadcast(EAttributeType::Health, GetHealthRatio());
	}

	if (CurrentHealth <= 0.f && OnDeath.IsBound())
	{
		OnDeath.Broadcast();
	}
}
