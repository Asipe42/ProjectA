// Copyright Modin


#include "Character/PACharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APACharacter::APACharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeCamera();
	InitializeCharacterMovement();
}

void APACharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APACharacter::InitializeCamera()
{
	// 1. SpringArm 생성 및 초기화
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	
	// 2. Camera 생성 및 초기화
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void APACharacter::InitializeCharacterMovement()
{

}