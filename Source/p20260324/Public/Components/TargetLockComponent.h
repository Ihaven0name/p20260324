#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetLockComponent.generated.h"

class AActor;
class APawn;

/** 控制器上的锁定系统，管理目标选择、切换和控制器朝向。 */
UCLASS(ClassGroup=(Player), meta=(BlueprintSpawnableComponent))
class P20260324_API UTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetLockComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="锁定", meta=(ClampMin="0.0"))
	float MaxLockDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="锁定", meta=(ClampMin="0.0", ClampMax="180.0"))
	float MaxLockAngle = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="锁定|切换", meta=(ClampMin="0.0"))
	float SwitchTargetThreshold = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="锁定|镜头" )
	float LockOnPitchRadius = -20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="锁定|镜头", meta=(ClampMin="0.0"))
	float RotationInterpSpeed = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category="锁定")
	bool bIsLocking = false;

	UPROPERTY(BlueprintReadOnly, Category="锁定")
	TObjectPtr<AActor> LockingActor = nullptr;

	UFUNCTION(BlueprintCallable, Category="锁定")
	void ToggleLockOn();

	UFUNCTION(BlueprintCallable, Category="锁定")
	void AddTargetSwitchInput(float AxisValue);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="锁定")
	AActor* GetLockingActor() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="锁定")
	bool IsLocking() const { return bIsLocking; }

	/** 角色切换或控制器失去本地控制时清理锁定状态。 */
	void ResetLock();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SetLockTarget(AActor* NewTarget);
	void UpdateLockOnRotation(float DeltaTime);

	TWeakObjectPtr<APawn> LockedPawn;
	float SwitchTargetInput = 0.0f;
	FRotator CurrentControlRotation = FRotator::ZeroRotator;
};
