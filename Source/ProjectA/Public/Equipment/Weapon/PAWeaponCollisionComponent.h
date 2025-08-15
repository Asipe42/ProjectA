// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PAWeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPAWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPAWeaponCollisionComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/** Hit Actor */
public:
	FOnHitActor OnHitActor;
	
/** Collision Trace */
public:
	void SetWeaponMesh(UPrimitiveComponent* NewWeaponMesh);
	void TurnOnCollision();
	void TurnOffCollision();

protected:
	void CollisionTrace();

	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;
	
	bool bIsCollisionEnabled = false;
	
/** Ignore */
public:
	FORCEINLINE void AddIgnoredActor(AActor* Actor) { IgnoredActors.Add(Actor); } 
	FORCEINLINE void RemoveIgnoredActor(AActor* Actor) { IgnoredActors.Remove(Actor); }

protected:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;
};
