// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "PAWeaponCollisionComponent.h"
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

public:
	APAWeapon();
	
protected:
	virtual void BeginPlay() override;
	
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

private:
	void InitializeCombat();
	
/** Attack */
public:
	float GetStaminaCost(const FGameplayTag& Tag) const;
	float GetAttackDamage() const;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;

/** Weapon Collision */
public:
	FORCEINLINE UPAWeaponCollisionComponent* GetWeaponCollisionComponent() const { return WeaponCollisionComponent; }

protected:
	void OnHitActor(const FHitResult& HitResult);
	
	UPROPERTY(VisibleAnywhere)
	UPAWeaponCollisionComponent* WeaponCollisionComponent;
	
private:
	void SetupWeaponCollision();
};
