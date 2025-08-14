// Copyright Modin


#include "Animation/Notify/PAFinishAttack.h"

#include "Character/PACharacter.h"

void UPAFinishAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APACharacter* Character = Cast<APACharacter>(MeshComp->GetOwner()))
	{
		Character->FinishAttack();
	}
}
