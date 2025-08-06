// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PACharacter.generated.h"

UCLASS()
class PROJECTA_API APACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APACharacter();

protected:
	virtual void BeginPlay() override;

private:
	void InitializeCamera();
	void InitializeCharacterMovement();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};
