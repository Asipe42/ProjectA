// Copyright Modin


#include "Equipment/PAWeapon.h"

#include "Combat/PACombatComponent.h"

void APAWeapon::Equip()
{
	Super::Equip();
	
	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);
		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
		AttachTo(AttachSocket);
	}
}

void APAWeapon::Unequip()
{
	Super::Unequip();
}

UAnimMontage* APAWeapon::GetMontageByTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageSet->GetMontageByTag(Tag, Index);
}

void APAWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* WeaponOwner = Cast<AActor>(GetOwner()))
	{
		CombatComponent = WeaponOwner->FindComponentByClass<UPACombatComponent>();
	}
}
