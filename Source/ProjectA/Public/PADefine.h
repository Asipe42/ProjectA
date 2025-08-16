// Copyright Modin

#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	None		UMETA(DisplayName = "None"),	
	Health		UMETA(DisplayName = "Health"),
	Stamina		UMETA(DisplayName = "Stamina"),
};

UENUM(BlueprintType)
enum class ETargetDirection : uint8
{
	Forward     UMETA(DisplayName = "Forward"),
	Left        UMETA(DisplayName = "Left"),
	Right       UMETA(DisplayName = "Right")
};