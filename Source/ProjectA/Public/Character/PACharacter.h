// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Attribute/PAAttributeComponent.h"
#include "Combat/PACombatComponent.h"
#include "State/PAStateComponent.h"
#include "UI/PAMainHUD.h"
#include "PACharacter.generated.h"

class UPATargetingComponent;
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

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

/** Components */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (AllowPrivateAccess = "true"))
	UPAAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UPAStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UPACombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (AllowPrivateAccess = "true"))
	UPATargetingComponent* TargetingComponent;

private:
	void SetupCamera();
	void SetupRotation();
	void SetupCharacterMovementComponent();
	void SetupAttributeComponent();
	void SetupStateComponent();
	void SetupCombatComponent();
	void SetupTargetingComponent();

/** Input System */
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();
	void Rolling();
	void Interact();
	void ToggleCombat();
	void EnterCombat();
	void Attack();
	void SpecialAttack();
	void HeavyAttack();
	void LockOn();
	void LockOnLeft();
	void LockOnRight();
	
	bool IsMoving() const;
	bool CanToggleCombat() const;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ToggleCombatAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* HeavyAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnLeftAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnRightAction;

private:
	void InitializeInputSystem();
	
/** HUD */
protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPAMainHUD> MainHUDClass;

	UPROPERTY()
	UPAMainHUD* MainHUDWidget;

private:
	void InitializeHUD();
	
/** Character Movement */
protected:
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
	
/** Attack */
public:
	void EnableComboWindow();
	void DisableComboWindow();
	void FinishAttack();
	
protected:
	void ExecuteComboAttack(const FGameplayTag& AttackType);
	bool CanExecuteComboAttack(const FGameplayTag& AttackType) const;
	void DoAttack(const FGameplayTag& AttackType);
	void ResetCombo();

	int32 ComboCounter = 0;
	bool bIsComboInputEnabled  = false;
	bool bIsComboInputQueued  = false;
};
