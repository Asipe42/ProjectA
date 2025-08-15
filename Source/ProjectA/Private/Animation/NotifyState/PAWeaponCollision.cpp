// Copyright Modin


#include "Animation/NotifyState/PAWeaponCollision.h"

#include "Combat/PACombatComponent.h"

void UPAWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UPACombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UPACombatComponent>())
		{
			const APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
			if (::IsValid(CurrentWeapon))
			{
				CurrentWeapon->GetWeaponCollisionComponent()->TurnOnCollision();
			}
		}
	}
}

void UPAWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const UPACombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UPACombatComponent>())
		{
			const APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
			if (::IsValid(CurrentWeapon))
			{
				CurrentWeapon->GetWeaponCollisionComponent()->TurnOffCollision();
			}
		}
	}
}
