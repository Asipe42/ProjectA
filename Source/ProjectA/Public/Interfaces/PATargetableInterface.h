// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PATargetableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPATargetableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTA_API IPATargetableInterface
{
	GENERATED_BODY()

public:
	virtual bool CanBeTargeted() = 0;
	virtual void OnTargeted(bool bIsTargeted) = 0;
};
