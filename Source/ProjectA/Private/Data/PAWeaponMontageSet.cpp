// Copyright Modin


#include "Data/PAWeaponMontageSet.h"

UAnimMontage* UPAWeaponMontageSet::GetMontageByTag(const FGameplayTag& Tag, const int32 Index) const
{
	if (const FPAWeaponMontageGroup* MontageGroup = MontageMap.Find(Tag))
	{
		if (MontageGroup->Montages.IsValidIndex(Index))
		{
			return MontageGroup->Montages[Index];
		}
	}
	
	return nullptr;
}
