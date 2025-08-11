// Copyright Modin


#include "UI/PAMainHUD.h"

#include "Attribute/PAAttributeComponent.h"

void UPAMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegate();
}

void UPAMainHUD::OnAttributeChange(EAttributeType AttributeType, float Ratio)
{
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

void UPAMainHUD::BindDelegate()
{
	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (UPAAttributeComponent* Attribute = OwningPawn->GetComponentByClass<UPAAttributeComponent>())
		{
			Attribute->OnAttributeChange.AddUObject(this, &UPAMainHUD::OnAttributeChange);
		}
	}
}
