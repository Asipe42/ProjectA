// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PAWeaponMontageSet.generated.h"

USTRUCT(BlueprintType)
struct FPAWeaponMontageGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

/**
 * 
 */
UCLASS()
class PROJECTA_API UPAWeaponMontageSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAnimMontage* GetMontageByTag(const FGameplayTag& Tag, const int32 Index) const;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName= "Montage Map")
	TMap<FGameplayTag, FPAWeaponMontageGroup> MontageMap;
};
