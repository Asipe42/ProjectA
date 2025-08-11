// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Attribute/PAAttributeComponent.h"
#include "State/PAStateComponent.h"
#include "UI/PAMainHUD.h"
#include "PACharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTA_API APACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APACharacter();

	/** Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	UPAAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UPAStateComponent* StateComponent;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
private:
	/** Setup */
	void SetupCamera();
	void SetupRotation();
	void SetupCharacterMovementComponent();
	void SetupAttributeComponent();
	void SetupStateComponent();

	/** Initialize */
	void InitializeInputSystem();
	void InitializeHUD();

	/** Input Handlers */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void Rolling();
	bool IsMoving() const;

	/** Input Actions */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RollingAction;

	/** HUD */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPAMainHUD> MainHUDClass;

	UPROPERTY()
	UPAMainHUD* MainHUDWidget;

	/** Character Movement */
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
	FRotator RotationRate = FRotator(0.f, 500.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
	float SprintSpeed = 800.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
	float DefaultWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Character Movement")
	UAnimMontage* RollingMontage;

	UPROPERTY()
	bool bIsSprinting = false;
};
