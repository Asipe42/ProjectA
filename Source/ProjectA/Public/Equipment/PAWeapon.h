// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Data/PAWeaponMontageSet.h"
#include "Equipment/PAEquipment.h"
#include "PAWeapon.generated.h"

class UPACombatComponent;
/**
 * 
 */
UCLASS()
class PROJECTA_API APAWeapon : public APAEquipment
{
	GENERATED_BODY()

	/** Equipment */
public:
	virtual void Equip() override;
	virtual void Unequip() override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName;}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	/** Montage Action */
public:
	UAnimMontage* GetMontageByTag(const FGameplayTag& Tag, const int32 Index = 0) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	UPAWeaponMontageSet* MontageSet;
	
	UPROPERTY()
	UPACombatComponent* CombatComponent;

/** Attack */
public:
	float GetStaminaCost(const FGameplayTag& Tag) const;
	
protected:
	UPROPERTY()
	TMap<FGameplayTag, float> StaminaCostMap;
	
protected:
	virtual void BeginPlay() override;
};
