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
	}
}

void APACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting && AttributeComponent)
	{
		AttributeComponent->DecreaseStamina(5.f * DeltaTime); 

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
	 *  - Proc 1. SpringArm 생성 및 초기화
	 *  - Proc 2. Camera 생성 및 초기화
	 */

	// Proc 1
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	
	// Proc 2
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
	check(StateComponent);
	
	if (!StateComponent->IsMovementEnabled())
		return;
	
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
	check(AttributeComponent);
	
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
	check(AttributeComponent);
	
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	AttributeComponent->RegenerateStamina(true);
}

void APACharacter::Rolling()
{
	check(StateComponent);
	check(AttributeComponent);

	if (!StateComponent->IsMovementEnabled())
		return;

	if (AttributeComponent->HasEnoughStamina(15.f))
	{
		PlayAnimMontage(RollingMontage);
		AttributeComponent->DecreaseStamina(15.f);
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

bool APACharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
		return MovementComp->Velocity.Size2D() > 0.3f && !MovementComp->GetCurrentAcceleration().IsNearlyZero();

	return false;
}
