// Copyright Modin


#include "Equipment/PAEquipment.h"

APAEquipment::APAEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupMeshComponent();
}

void APAEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateMeshComponent();
}

void APAEquipment::Equip()
{
}

void APAEquipment::Unequip()
{
}

void APAEquipment::AttachTo(const FName SocketName)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
		return;

	USkeletalMeshComponent* SkeletalMeshComp = OwnerPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMeshComp)
		return;

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(SkeletalMeshComp, AttachRules, SocketName);
}

UStaticMesh* APAEquipment::GetEquipmentMeshAsset() const
{
	return MeshAsset;
}

void APAEquipment::SetupMeshComponent()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void APAEquipment::UpdateMeshComponent()
{
	if (MeshAsset)
	{
		MeshComponent->SetStaticMesh(MeshAsset);
		MeshComponent->SetVisibility(true);
	}
	else
	{
		MeshComponent->SetStaticMesh(nullptr);
		MeshComponent->SetVisibility(false);
	}
}
