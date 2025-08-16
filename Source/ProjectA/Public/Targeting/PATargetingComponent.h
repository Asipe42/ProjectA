// Copyright Modin

#pragma once

#include "CoreMinimal.h"
#include "PADefine.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PATargetingComponent.generated.h"


class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UPATargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPATargetingComponent();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UCameraComponent* OwnerCamera;

private:
	void InitializeCharacter();
	void InitializeCamera();
	
/** Lock-On */
public:
	void ToggleLockOn();
	void SwitchTargetLeft();
	void SwitchTargetRight();

protected:
	void EnableLockOn();
	void DisableLockOn();

	void EnableCameraBasedRotation();
	void EnableMovementBasedRotation();

	bool IsValidTarget() const;
	void RotateTowardsTarget();

	TArray<AActor*> GetLockOnCandidates() const;
	AActor* GetClosestTarget(const TArray<AActor*>& Candidates, const ETargetDirection Direction) const;
	
	bool bIsLockOn = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Lock-On")
	AActor* LockedTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Lock-On")
	float LockOnRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Lock-On")
	float RotationSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
};
