// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "PADefine.h"
#include "Components/ActorComponent.h"
#include "PAAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangeSignature, EAttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPAAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPAAttributeComponent();

/** Event */
public:
	FOnAttributeChangeSignature OnAttributeChange;
	FOnDeath OnDeath;
	
/** Stamina */
public:
	void ModifyStamina(float Amount);
	void RegenerateStamina(bool bEnable, float Delay = 0.0f);
	bool HasEnoughStamina(float Amount) const;

	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE float GetStaminaRatio() const { return CurrentStamina / MaxStamina; }

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float RegenerateStaminaRate = 0.5f;

private:
	FTimerHandle RegenerationTimerHandle;

/** Health */
public:
	void ModifyHealth(float Amount);
	
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetHealthRatio() const { return CurrentHealth / MaxHealth; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;
};