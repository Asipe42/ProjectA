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
	void SetWeapon(APAWeapon* NewWeapon);
	FORCEINLINE APAWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	/** Combat Enable */
	FORCEINLINE void SetCombatEnabled(const bool bEnable) { bIsCombatEnabled = bEnable; }
	FORCEINLINE bool IsCombatEnabled() const { return bIsCombatEnabled; }

protected:
	UPROPERTY()
	APAWeapon* CurrentWeapon;

private:
	UPROPERTY(EditAnywhere)
	bool bIsCombatEnabled = true;
};
