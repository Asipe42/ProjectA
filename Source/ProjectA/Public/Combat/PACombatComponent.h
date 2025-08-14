// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Equipment/PAWeapon.h"
#include "PACombatComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPACombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPACombatComponent();

/** Weapon */
public:
	void SetWeapon(APAWeapon* NewWeapon);
	FORCEINLINE APAWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
	UPROPERTY()
	APAWeapon* CurrentWeapon;
	
/** Combat Enable */
public:
	FORCEINLINE void SetCombatEnabled(const bool bEnable) { bIsCombatEnabled = bEnable; }
	FORCEINLINE bool IsCombatEnabled() const { return bIsCombatEnabled; }

private:
	UPROPERTY(EditAnywhere)
	bool bIsCombatEnabled = true;

/** Attack */
public:
	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; };

protected:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;
};
