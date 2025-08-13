// Copyright Modin


#include "Combat/PACombatComponent.h"

#include "Character/PACharacter.h"
#include "Item/PAPickupItem.h"

UPACombatComponent::UPACombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPACombatComponent::SetWeapon(APAWeapon* NewWeapon)
{
	/*
	 * 무기 설정
	 *  - Case A: 장착 중인 무기가 없는 경우
	 *	- Case B: 장착 중인 무기가 있는 경우
	 *		장착 중인 무기를 PickupItem으로 만든 후 제거한다.
	 */
	
	if (::IsValid(CurrentWeapon))
	{
		// Case B
		if (APACharacter* OwnerCharacter = Cast<APACharacter>(GetOwner()))
		{
			const FTransform SpawnTransform = OwnerCharacter->GetActorTransform();

			APAPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<APAPickupItem>
			(
				APAPickupItem::StaticClass(),
				SpawnTransform,
				nullptr,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);

			ensure(CurrentWeapon);
			PickupItem->SetEquipmentClass(CurrentWeapon->GetClass());
			PickupItem->FinishSpawning(SpawnTransform);

			CurrentWeapon->Destroy();
			CurrentWeapon = nullptr;
		}
	}

	// Case A
	CurrentWeapon = NewWeapon;
}
