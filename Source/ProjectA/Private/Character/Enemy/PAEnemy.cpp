// Copyright Modin


#include "Character/Enemy/PAEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APAEnemy::APAEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	SetupCapsuleCollision();
	SetupMeshComponent();
	SetupAttributeComponent();
	SetupStateComponent();
}

void APAEnemy::SetupCapsuleCollision()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void APAEnemy::SetupMeshComponent()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void APAEnemy::SetupAttributeComponent()
{
	AttributeComponent = CreateDefaultSubobject<UPAAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void APAEnemy::SetupStateComponent()
{
	StateComponent = CreateDefaultSubobject<UPAStateComponent>(TEXT("StateComponent"));
}

float APAEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ensure(AttributeComponent);

	/*
	 * 피격
	 *  Proc 1: 체력 차감 (체력이 0 이하라면 사망)
	 *  Proc 2: 피격 이펙트 재생
	 *  Proc 3: 피격 사운드 재생
	 *  Proc 4: 피격 리액션 재생
	 */
	
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	AttributeComponent->ModifyHealth(-ActualDamage);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		const FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		const FVector InstigatorLocation = EventInstigator->GetPawn()->GetActorLocation();

		PlayHitEffect(ImpactPoint);
		PlayHitSound(ImpactPoint);
		PlayHitReaction(InstigatorLocation);
	}
	
	return ActualDamage;
}

void APAEnemy::OnDeath()
{
	/*
	 * 사망
	 *	Proc 1: 상태 컴포넌트 비활성화
	 *	Proc 2: 콜리전 비활성화
	 *	Proc 3: RagDoll 활성화
	 */

	ensure(StateComponent);

	StateComponent->SetState(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Death")));
	
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
	}
}

void APAEnemy::PlayHitEffect(const FVector& Location)
{
	if (!HitParticle) 
		return;

	UWorld* World = GetWorld();
	if (!World) 
		return;
	
	UGameplayStatics::SpawnEmitterAtLocation(World, HitParticle, Location, FRotator::ZeroRotator, true);
}

void APAEnemy::PlayHitSound(const FVector& Location)
{
	if (!HitSound)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	UGameplayStatics::PlaySoundAtLocation(World, HitSound, Location);
}

void APAEnemy::PlayHitReaction(const FVector& InstigatorLocation)
{
    const FVector EnemyForward = GetActorForwardVector();
    const FVector ToInstigator = (InstigatorLocation - GetActorLocation()).GetSafeNormal();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(EnemyForward, ToInstigator)));
	
    UAnimMontage* MontageToPlay;
    if (Angle <= 45.f)
    {
        MontageToPlay = HitReactAnimFront;
    }
    else if (Angle < 135.f)
    {
    	/*
		 * 설명
		 *  - 두 벡터 A, B가 있다.
		 *  - 외적의 Z 성분이 양수라면 A에서 B로 회전하는 방향이 반시계 방향이다.
		 *  - 외적의 Z 성분이 음수라면 A에서 B로 회전하는 방향이 시계 방향이다.
		 *  - 외적의 Z 성분이 0이라면 두 벡터는 평행이다.
		 */
    	
    	const float CrossZ = FVector::CrossProduct(EnemyForward, ToInstigator).Z;
        MontageToPlay = (CrossZ > 0.f) ? HitReactAnimLeft : HitReactAnimRight;
    }
    else
    {
        MontageToPlay = HitReactAnimBack;
    }

    if (MontageToPlay)
    {
        PlayAnimMontage(MontageToPlay);
    }
}