// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PAAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPAAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPAAttributeComponent();

	FORCEINLINE float GetBaseStamina() const { return CurrentStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	void DecreaseStamina(float Amount);
	bool HasEnoughStamina(float Amount) const;
	
protected:

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina;
};