// Copyright Modin


#include "Targeting/PATargetingComponent.h"

#include "PADefine.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/PATargetableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UPATargetingComponent::UPATargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPATargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsLockOn || !IsValid(OwnerCharacter) || !IsValid(LockedTarget))
		return;

	if (IsValidTarget())
		RotateTowardsTarget();
	else
		DisableLockOn();
}

void UPATargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacter();
	InitializeCamera();
}

void UPATargetingComponent::InitializeCharacter()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UPATargetingComponent::InitializeCamera()
{
	if (!OwnerCharacter)
		return;

	OwnerCamera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
}

void UPATargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		DisableLockOn();
	}
	else
	{
		EnableLockOn();
	}
}

void UPATargetingComponent::SwitchTargetLeft()
{
}

void UPATargetingComponent::SwitchTargetRight()
{
}

void UPATargetingComponent::EnableLockOn()
{
	/*
	 * 록온 활성화
	 *	Proc 1: 전방으로 원형 범위의 트레이스를 실행하여 후보군 생성
	 *  Proc 2: 후보군 중 각도가 가장 가까운 대상 선별
	 *  Proc 3: 선별된 대상이 록온이 가능한 대상인지 검사
	 *  Proc 4: 록온 활성화
	 *  Proc 5: 카메라가 타겟을 바라보도록 고정
	 */

	const TArray<AActor*> Candidates = GetLockOnCandidates();
	AActor* ClosestTarget = GetClosestTarget(Candidates, ETargetDirection::Forward);

	if (IsValid(ClosestTarget))
	{
		bIsLockOn = true;
		LockedTarget = ClosestTarget;

		if (IPATargetableInterface* Target = Cast<IPATargetableInterface>(LockedTarget))
		{
			Target->OnTargeted(bIsLockOn);
		}
	}

	EnableCameraBasedRotation();
}

void UPATargetingComponent::DisableLockOn()
{
	bIsLockOn = false;
	if (IPATargetableInterface* Targeting = Cast<IPATargetableInterface>(LockedTarget))
	{
		Targeting->OnTargeted(bIsLockOn);
	}
	LockedTarget = nullptr;

	EnableMovementBasedRotation();
}

void UPATargetingComponent::EnableCameraBasedRotation()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UPATargetingComponent::EnableMovementBasedRotation()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

bool UPATargetingComponent::IsValidTarget() const
{
	const float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), LockedTarget->GetActorLocation());

	if (IPATargetableInterface* Targeting = Cast<IPATargetableInterface>(LockedTarget))
	{
		return Targeting->CanBeTargeted() && Distance <= LockOnRange;
	}

	return false;
}

void UPATargetingComponent::RotateTowardsTarget()
{
	const FRotator CurrentControlRotation = OwnerCharacter->GetControlRotation();
	const FVector TargetLocation = LockedTarget->GetActorLocation() - FVector(0.f, 0.f, 150.f);
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), TargetLocation);
	const FRotator InterpRotation = FMath::RInterpTo(CurrentControlRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed);

	OwnerCharacter->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, CurrentControlRotation.Roll));
}

TArray<AActor*> UPATargetingComponent::GetLockOnCandidates() const
{
	TArray<AActor*> Candidates;

	if (!OwnerCharacter)
	{
		return Candidates;
	}
	
	const FVector Start = OwnerCharacter->GetActorLocation();
	const FVector End = Start;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_TARGETING));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);

	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		GetWorld(),
		Start,
		End,
		LockOnRange,
		ObjectTypes,
		false,
		IgnoreActors,
		DrawDebugType,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const FHitResult& HitResult : OutHits)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IPATargetableInterface* Targeting = Cast<IPATargetableInterface>(HitActor))
			{
				if (Targeting->CanBeTargeted())
				{
					Candidates.Add(HitActor);
				}
			}
		}
	}

	return Candidates;
}

AActor* UPATargetingComponent::GetClosestTarget(const TArray<AActor*>& Candidates, const ETargetDirection Direction) const
{
	float TargetCompareValue = 0.f;
	AActor* ClosestTarget = nullptr;

	for (const AActor* TargetActor : Candidates)
	{
		if (LockedTarget == TargetActor)
		{
			continue;
		}

		FHitResult OutHit;
		const FVector Start = OwnerCamera->GetComponentLocation();
		const FVector End = TargetActor->GetActorLocation();

		TArray<AActor*> ActorsToIgnore;
		if (LockedTarget)
			ActorsToIgnore.Add(LockedTarget);
		
		const bool bHit = UKismetSystemLibrary::LineTraceSingle
		(
			GetOwner(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			DrawDebugType,
			OutHit,
			true
		);

		if (bHit)
		{
			if (Direction == ETargetDirection::Left)
			{
				if (FVector::DotProduct(OwnerCamera->GetRightVector(), OutHit.Normal) > 0.f == false)
					continue;
			}

			if (Direction == ETargetDirection::Right)
			{
				if (FVector::DotProduct(OwnerCamera->GetRightVector(), OutHit.Normal) < 0.f == false)
					continue;
			}

			AActor* HitActor = OutHit.GetActor();
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), HitActor->GetActorLocation());
			const float CheckValue = FMath::Abs(FVector::DotProduct(OwnerCamera->GetForwardVector(), LookAtRotation.Vector()));
			
			if (CheckValue > TargetCompareValue)
			{
				TargetCompareValue = CheckValue;
				ClosestTarget = HitActor;
			}
		}
	}

	return ClosestTarget;
}
