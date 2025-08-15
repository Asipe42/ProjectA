// Copyright Modin


#include "Equipment/Weapon/PAWeapon.h"

#include "Combat/PACombatComponent.h"
#include "Kismet/GameplayStatics.h"

APAWeapon::APAWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetupWeaponCollision();
}

void APAWeapon::BeginPlay()
{
	Super::BeginPlay();

	InitializeCombat();
}

void APAWeapon::Equip()
{
	Super::Equip();

	/*
	 * 장착
	 *	Proc 1: CombatComponent에 무기 설정
	 *	Proc 2: CombatComponent가 활성화된 상태라면 EquipSocketName에 장착
	 *	Proc 3: WeaponCollisionComponent에 Mesh 전달 및 무기 소유자 무시 설정
	 */
	
	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);
		
		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
		AttachTo(AttachSocket);

		WeaponCollisionComponent->SetWeaponMesh(MeshComponent);
		WeaponCollisionComponent->AddIgnoredActor(GetOwner());
	}
}

void APAWeapon::Unequip()
{
	Super::Unequip();
}

UAnimMontage* APAWeapon::GetMontageByTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageSet->GetMontageByTag(Tag, Index);
}

void APAWeapon::InitializeCombat()
{
	if (AActor* WeaponOwner = Cast<AActor>(GetOwner()))
	{
		CombatComponent = WeaponOwner->FindComponentByClass<UPACombatComponent>();
	}
}

float APAWeapon::GetStaminaCost(const FGameplayTag& Tag) const
{
	if (const float* Cost = StaminaCostMap.Find(Tag))
	{
		return *Cost;
	}

	return 0.f;
}

float APAWeapon::GetAttackDamage() const
{
	float FinalDamage = BaseDamage;

	const AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return FinalDamage;
	}

	const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();
	if (DamageMultiplierMap.Contains(LastAttackType))
	{
		const float Multiplier = DamageMultiplierMap[LastAttackType];
		return BaseDamage * Multiplier;
	}
	
	return FinalDamage;
}

void APAWeapon::OnHitActor(const FHitResult& HitResult)
{
	AActor* TargetActor = HitResult.GetActor();
	const FVector DamageDirection = GetOwner()->GetActorForwardVector();
	const float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage
	(
		TargetActor,
		AttackDamage,
		DamageDirection,
		HitResult,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr
	);
}

void APAWeapon::SetupWeaponCollision()
{
	WeaponCollisionComponent = CreateDefaultSubobject<UPAWeaponCollisionComponent>("WeaponCollision");
	WeaponCollisionComponent->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);
}
