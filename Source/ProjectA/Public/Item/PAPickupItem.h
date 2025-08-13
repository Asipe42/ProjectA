// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Equipment/PAEquipment.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PAInteractableInterface.h"
#include "PAPickupItem.generated.h"

UCLASS()
class PROJECTA_API APAPickupItem : public AActor, public IPAInteractableInterface
{
	GENERATED_BODY()
	
public:	
	APAPickupItem();

	virtual void Interact(AActor* Interactor) override;
	FORCEINLINE void SetEquipmentClass(TSubclassOf<APAEquipment> NewClass) { EquipmentClass = NewClass; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSubclassOf<APAEquipment> EquipmentClass;

private:
	void SetupMeshComponent();
	
	void UpdateMeshComponent();
};
