// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PAInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPAInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTA_API IPAInteractableInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(AActor* Interactor) = 0;
};
