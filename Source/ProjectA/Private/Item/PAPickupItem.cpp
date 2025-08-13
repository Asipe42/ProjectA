// Copyright Modin


#include "Item/PAPickupItem.h"

#include "PADefine.h"

APAPickupItem::APAPickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupMeshComponent();
}

void APAPickupItem::Interact(AActor* Interactor)
{
	if (!EquipmentClass || !Interactor)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Interactor;

	APAEquipment* SpawnedEquipment = World->SpawnActor<APAEquipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (!SpawnedEquipment)
		return;

	SpawnedEquipment->Equip();
	Destroy();
}

void APAPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateMeshComponent();
}

void APAPickupItem::SetupMeshComponent()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	MeshComponent->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APAPickupItem::UpdateMeshComponent()
{
	if (!MeshComponent)
		return;
	
	if (!EquipmentClass)
		return;
		
	if (APAEquipment* CDO = EquipmentClass->GetDefaultObject<APAEquipment>())
	{
		MeshComponent->SetStaticMesh(CDO->GetEquipmentMeshAsset());
		MeshComponent->SetSimulatePhysics(true);
	}
}
