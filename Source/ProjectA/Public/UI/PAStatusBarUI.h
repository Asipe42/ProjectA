// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PAStatusBarUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UPAStatusBarUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFillColorAndOpacity(const FLinearColor& Color);
	void SetRatio(const float Ratio);

protected:
	virtual void NativePreConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

private:
	UPROPERTY(EditAnywhere, Category = "Status Bar")
	FLinearColor DefaultFillColor = FLinearColor::Red;
};
