// Copyright Modin

#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	None		UMETA(DisplayName = "None"),	
	Health		UMETA(DisplayName = "Health"),
	Stamina		UMETA(DisplayName = "Stamina"),
};