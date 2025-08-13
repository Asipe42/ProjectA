// Copyright Modin


#include "Animation/AnimNotify_PAEquipWeapon.h"

#include "Combat/PACombatComponent.h"

void UAnimNotify_PAEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	/*
	 * Notify
	 *  - Proc 1: 전투 상태로 전환
	 *  - Proc 2: 무기 장착/해제
	 */

	const AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;

	UPACombatComponent* CombatComponent = Owner->FindComponentByClass<UPACombatComponent>();
	if (!CombatComponent)
		return;

	APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
	if (!CurrentWeapon)
		return;

	bool bCombatEnabled = CombatComponent->IsCombatEnabled();
	FName SocketName;

	if (TargetTag == FGameplayTag::RequestGameplayTag(TEXT("Character.Action.Equip")))
	{
		bCombatEnabled = true;
		SocketName = CurrentWeapon->GetEquipSocketName();
	}
	else if (TargetTag == FGameplayTag::RequestGameplayTag(TEXT("Character.Action.Unequip")))
	{
		bCombatEnabled = false;
		SocketName = CurrentWeapon->GetUnequipSocketName();
	}

	// Proc 1: 전투 상태로 전환
	CombatComponent->SetCombatEnabled(bCombatEnabled);

	// Proc 2: 무기 장착/해제
	CurrentWeapon->AttachTo(SocketName);
}
