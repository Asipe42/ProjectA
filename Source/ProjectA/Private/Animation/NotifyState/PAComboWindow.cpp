// Copyright Modin


#include "Animation/NotifyState/PAComboWindow.h"

#include "Character/PACharacter.h"

void UPAComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (APACharacter* Character = Cast<APACharacter>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UPAComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (APACharacter* Character = Cast<APACharacter>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
