// Copyright Modin


#include "Character/PACharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interfaces/PAInteractableInterface.h"
#include "Kismet/KismetSystemLibrary.h"

APACharacter::APACharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupRotation();
	SetupCamera();
	SetupCharacterMovementComponent();
	SetupAttributeComponent();
	SetupStateComponent();
	SetupCombatComponent();
}

void APACharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeInputSystem();
	InitializeHUD();
}

void APACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APACharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APACharacter::Look);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APACharacter::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APACharacter::StopSprint);
		EnhancedInput->BindAction(RollingAction, ETriggerEvent::Started, this, &APACharacter::Rolling);
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &APACharacter::Interact);
		EnhancedInput->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &APACharacter::ToggleCombat);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &APACharacter::EnterCombat);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APACharacter::SpecialAttack);
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Canceled, this, &APACharacter::Attack);
		EnhancedInput->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &APACharacter::EnterCombat);
		EnhancedInput->BindAction(HeavyAttackAction, ETriggerEvent::Completed, this, &APACharacter::HeavyAttack);
	}
}

void APACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting && AttributeComponent)
	{
		AttributeComponent->ModifyStamina(-5.f * DeltaTime); 

		if (!AttributeComponent->HasEnoughStamina(0.1f))
		{
			StopSprint();
		}
	}
}

void APACharacter::SetupRotation()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void APACharacter::SetupCamera()
{
	/*
	 * 카메라 설정
	 *	Proc 1. SpringArm 생성 및 초기화
	 *	Proc 2. Camera 생성 및 초기화
	 */

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
}

void APACharacter::SetupCharacterMovementComponent()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->RotationRate = RotationRate;
}

void APACharacter::SetupAttributeComponent()
{
	AttributeComponent = CreateDefaultSubobject<UPAAttributeComponent>(TEXT("Attribute"));
}

void APACharacter::SetupStateComponent()
{
	StateComponent = CreateDefaultSubobject<UPAStateComponent>(TEXT("State"));
}

void APACharacter::SetupCombatComponent()
{
	CombatComponent = CreateDefaultSubobject<UPACombatComponent>(TEXT("Combat"));
}

void APACharacter::InitializeInputSystem()
{
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void APACharacter::InitializeHUD()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (!MainHUDClass)
			return;
		
		MainHUDWidget = CreateWidget<UPAMainHUD>(PC, MainHUDClass);
		
		if (MainHUDWidget)
			MainHUDWidget->AddToViewport();
	}
}

void APACharacter::Move(const FInputActionValue& Value)
{
	ensure(StateComponent);
	
	if (!StateComponent->IsMovementEnabled())
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APACharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void APACharacter::StartSprint()
{
	ensure(AttributeComponent);
	
	if (AttributeComponent->HasEnoughStamina(5.f) && IsMoving())
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		AttributeComponent->RegenerateStamina(false);
	}
	else
	{
		StopSprint();
	}
}

void APACharacter::StopSprint()
{
	ensure(AttributeComponent);
	
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	AttributeComponent->RegenerateStamina(true);
}

void APACharacter::Rolling()
{
	ensure(StateComponent);
	ensure(AttributeComponent);

	if (!StateComponent->IsMovementEnabled())
	{
		return;
	}
	
	if (AttributeComponent->HasEnoughStamina(15.f))
	{
		PlayAnimMontage(RollingMontage);
		AttributeComponent->ModifyStamina(-15.f);
		AttributeComponent->RegenerateStamina(false);
		StateComponent->SetMovementEnabled(false);
		StateComponent->SetState(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Rolling")));
	}
}

void APACharacter::Interact()
{
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = StartLocation;
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	FHitResult OutHit;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		this,
		StartLocation,
		EndLocation,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true
	);

	if (bHit)
	{
		AActor* HitActor = OutHit.GetActor();
		if (HitActor && HitActor->GetClass())
		{
			if (IPAInteractableInterface* Interaction = Cast<IPAInteractableInterface>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void APACharacter::ToggleCombat()
{
	ensure(CombatComponent);
	ensure(StateComponent);

	/*
	 * 전투 토글
	 *	Case A: 전투가 활성화된 상태 → Unequip 몽타주 재생
	 *	Case B: 전투가 비활성화된 상태 → Equip 몽타주 재생
	 */

	const APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
	if (!CurrentWeapon)
	{
		return;
	}
		
	if (CanToggleCombat())
	{
		StateComponent->SetState(FGameplayTag::RequestGameplayTag(TEXT("Character.State.GeneralAction")));

		const FGameplayTag MontageTag = FGameplayTag::RequestGameplayTag
		(
			CombatComponent->IsCombatEnabled()
			? TEXT("Character.Action.Unequip") // Case A
			: TEXT("Character.Action.Equip")   // Case B
		);
		
		UAnimMontage* Montage = CurrentWeapon->GetMontageByTag(MontageTag);
		PlayAnimMontage(Montage);
	}
}

void APACharacter::EnterCombat()
{
	ensure(CombatComponent);
	
	if (!CombatComponent->IsCombatEnabled())
	{
		ToggleCombat();
	}
}

void APACharacter::Attack()
{
	/*
	 * 우선 순위
	 *	Case A: 질주 중일 경우, 돌진 공격
	 *	Case B: 그 외의 경우, 일반 공격
	 */
	
	FGameplayTag AttackType;

	if (bIsSprinting)
	{
		// Case A
		AttackType = FGameplayTag::RequestGameplayTag(TEXT("Character.Attack.Running"));
	}
	else
	{
		// Case B
		AttackType = FGameplayTag::RequestGameplayTag(TEXT("Character.Attack.Light"));
	}
	
	if (!CanExecuteComboAttack(AttackType))
	{
		return;
	}

	ExecuteComboAttack(AttackType);
}

void APACharacter::SpecialAttack()
{
	FGameplayTag AttackType = FGameplayTag::RequestGameplayTag(TEXT("Character.Attack.Special"));

	if (!CanExecuteComboAttack(AttackType))
	{
		return;
	}

	ExecuteComboAttack(AttackType);
}

void APACharacter::HeavyAttack()
{
	FGameplayTag AttackType = FGameplayTag::RequestGameplayTag(TEXT("Character.Attack.Heavy"));
	
	if (!CanExecuteComboAttack(AttackType))
	{
		return;
	}

	ExecuteComboAttack(AttackType);
}

bool APACharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 0.3f && !MovementComp->GetCurrentAcceleration().IsNearlyZero();
	}
	
	return false;
}

bool APACharacter::CanToggleCombat() const
{
	ensure(StateComponent);

	FGameplayTagContainer BlockStates;
	BlockStates.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Attacking")));
	BlockStates.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Rolling")));
	BlockStates.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.GeneralAction")));

	return !StateComponent->IsCurrentStateIn(BlockStates);
}

void APACharacter::EnableComboWindow()
{
	 bIsComboInputEnabled = true;
}

void APACharacter::DisableComboWindow()
{
	ensure(CombatComponent);
	
	bIsComboInputEnabled = false;

	if (!bIsComboInputQueued)
	{
		return;
	}

	bIsComboInputQueued = false;
	ComboCounter++;
	DoAttack(CombatComponent->GetLastAttackType());
}

void APACharacter::FinishAttack()
{
	ensure(StateComponent);
	
	StateComponent->SetMovementEnabled(true);
	StateComponent->ClearState();
	ResetCombo();
}

void APACharacter::ExecuteComboAttack(const FGameplayTag& AttackType)
{
	/*
	 * 콤보 공격
	 *	Case A: 공격 중인 경우
	 *	Case B: 공격 중이 아닌 경우
	 */

	ensure(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Attacking")));

	if (StateComponent->IsCurrentStateIn(CheckTags))
	{
		// Case A
		if (bIsComboInputEnabled)
		{
			bIsComboInputQueued = true;
		}
		
		return;
	}

	// Case B
	ResetCombo();
	DoAttack(AttackType);
}

bool APACharacter::CanExecuteComboAttack(const FGameplayTag& AttackType) const
{
	ensure(StateComponent);
	ensure(CombatComponent);
	ensure(AttributeComponent);

	/*
	 * 콤보 공격 조건
	 *	Cond 1: 무기를 장착 중인 상태
	 *	Cond 2: 전투가 활성화된 상태
	 *	Cond 3: 현재 상태가 공격 가능한 상태
	 *	Cond 4: 스태미나가 충분한 상태
	 */
	
	const APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
	if (!IsValid(CurrentWeapon))
	{
		return false;
	}

	if (!CombatComponent->IsCombatEnabled())
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Rolling")));
	CheckTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.GeneralAction")));
	if (StateComponent->IsCurrentStateIn(CheckTags))
	{
		return false;
	}

	const float StaminaCost = CurrentWeapon->GetStaminaCost(AttackType);
	if (!AttributeComponent->HasEnoughStamina(StaminaCost))
	{
		return false;
	}
	
	return true;
}

void APACharacter::DoAttack(const FGameplayTag& AttackType)
{
	ensure(StateComponent);
	ensure(AttributeComponent);
	ensure(CombatComponent);

	/*
	 * 공격
	 *	Proc 1: Attack Type 캐싱
	 *	Proc 2: '공격 중'으로 상태 변경
	 *	Proc 3: 입력 차단
	 *	Proc 4: 스태미나 소모
	 *	Proc 5: 스태미나 재생 중단
	 *	Proc 6: 스태미나 재생 예약
	 *	Proc 7: 몽타주 재생
	 */

	const APAWeapon* CurrentWeapon = CombatComponent->GetCurrentWeapon();
	if (!IsValid(CurrentWeapon))
	{
		return;
	}

	CombatComponent->SetLastAttackType(AttackType);
	
	StateComponent->SetState(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Attacking")));
	StateComponent->SetMovementEnabled(false);

	const float StaminaCost = CurrentWeapon->GetStaminaCost(AttackType) * -1.f;
	AttributeComponent->ModifyStamina(StaminaCost);
	AttributeComponent->RegenerateStamina(false);
	AttributeComponent->RegenerateStamina(true, 1.5f);

	UAnimMontage* Montage = CurrentWeapon->GetMontageByTag(AttackType, ComboCounter);
	if (!Montage)
	{
		ComboCounter = 0;
		Montage = CurrentWeapon->GetMontageByTag(AttackType, 0);
	}
	PlayAnimMontage(Montage);
}

void APACharacter::ResetCombo()
{
	ComboCounter = 0;
	bIsComboInputEnabled = false;
	bIsComboInputQueued = false;
}