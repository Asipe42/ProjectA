// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "PADefine.h"
#include "PAStatusBarUI.h"
#include "Blueprint/UserWidget.h"
#include "PAMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UPAMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnAttributeChange(EAttributeType AttributeType, float CurrentValue, float MaxValue);
	
protected:
	/** Health */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPAStatusBarUI* HealthBar;

	/** Stamina */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPAStatusBarUI* StaminaBar;
};
