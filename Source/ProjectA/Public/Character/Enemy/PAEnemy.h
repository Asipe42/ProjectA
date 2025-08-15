// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Attribute/PAAttributeComponent.h"
#include "GameFramework/Character.h"
#include "State/PAStateComponent.h"
#include "PAEnemy.generated.h"

UCLASS()
class PROJECTA_API APAEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	APAEnemy();

protected:
	UPROPERTY(VisibleAnywhere)
	UPAAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UPAStateComponent* StateComponent;

private:
	void SetupCapsuleCollision();
	void SetupMeshComponent();
	void SetupAttributeComponent();
	void SetupStateComponent();
	
/** Hit */
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath();

protected:
	void PlayHitEffect(const FVector& Location);
	void PlayHitSound(const FVector& Location);
	void PlayHitReaction(const FVector& InstigatorLocation);

	UPROPERTY(EditAnywhere, Category = "Hit Effect")
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere, Category = "Hit Effect")
	UParticleSystem* HitParticle;
	
	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Hit Reaction")
	UAnimMontage* HitReactAnimRight;
};
