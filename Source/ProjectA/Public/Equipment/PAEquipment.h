// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PAEquipment.generated.h"

UCLASS()
class PROJECTA_API APAEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	APAEquipment();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Equip();
	virtual void Unequip();
	virtual void AttachTo(const FName SocketName);

	UFUNCTION(BlueprintPure, Category = "Equipment")
	UStaticMesh* GetEquipmentMeshAsset() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	UStaticMeshComponent* MeshComponent;

private:
	void SetupMeshComponent();
	void UpdateMeshComponent();
};
