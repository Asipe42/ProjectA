// Copyright Modin


#include "Character/PACharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APACharacter::APACharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SetupRotation();
	SetupCamera();
	SetupCharacterMovement();
	SetupAttribute();
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
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APACharacter::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APACharacter::StopSprint);
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
	 * 카메라 초기화
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

void APACharacter::SetupCharacterMovement()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->RotationRate = RotationRate;
}

void APACharacter::SetupAttribute()
{
	Attribute = CreateDefaultSubobject<UPAAttributeComponent>(TEXT("Attribute"));
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
	if (Attribute)
	{
		if (Attribute->HasEnoughStamina(5.f) && CanSprint())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			Attribute->RegenerateStamina(false);
			Attribute->DecreaseStamina(0.1f);
		}
		else
			StopSprint();
	}
}

void APACharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	if (Attribute)
		Attribute->RegenerateStamina(true);
}

bool APACharacter::CanSprint() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
		return MovementComp->Velocity.Size2D() > 0.3f && !MovementComp->GetCurrentAcceleration().IsNearlyZero();

	return false;
}