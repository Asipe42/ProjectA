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

bool UPAAttributeComponent::HasEnoughStamina(const float Amount) const
{
	return CurrentStamina >= Amount;
}
