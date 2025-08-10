// Copyright Modin


#include "UI/PAMainHUD.h"

void UPAMainHUD::OnAttributeChange(EAttributeType AttributeType, const float CurrentValue, const float MaxValue)
{
	const float Ratio = MaxValue > 0.f ? CurrentValue / MaxValue : 0.f;
	switch (AttributeType)
	{ 
		case EAttributeType::Health:
			if (HealthBar)
				HealthBar->SetRatio(Ratio);
			break;

		case EAttributeType::Stamina:
			if (StaminaBar)
				StaminaBar->SetRatio(Ratio);
			break;
		
		default:
			break;
	}
}
