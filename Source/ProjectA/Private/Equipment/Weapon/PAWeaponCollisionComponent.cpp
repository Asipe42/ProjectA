// Copyright Modin


#include "Equipment/Weapon/PAWeaponCollisionComponent.h"

UPAWeaponCollisionComponent::UPAWeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPAWeaponCollisionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CollisionTrace();
}

void UPAWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* NewWeaponMesh)
{
	ensure(NewWeaponMesh);
	
	WeaponMesh = NewWeaponMesh;
}

void UPAWeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UPAWeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UPAWeaponCollisionComponent::CollisionTrace()
{
	if (!bIsCollisionEnabled || !WeaponMesh)
		return;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		GetWorld(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		HitResults,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor)
				continue;
			
			if (AlreadyHitActors.Contains(HitActor))
				continue;

			AlreadyHitActors.Add(HitActor);

			if (OnHitActor.IsBound())
			{
				OnHitActor.Broadcast(Hit);
			}
		}
	}
}
