#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitReleaseAtDuration.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAtDurationEndSignature,bool,bReleaseAtDuration);



UCLASS()
class P20260324_API UAbilityTask_WaitReleaseAtDuration : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAtDurationEndSignature OnAtDurationEndSignature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TaskDuration;
	
	UPROPERTY()
	bool bIsReleased=false;
	UPROPERTY()
	float StartTime=0.f;
	UPROPERTY()
	bool bTestInitialState=false;
	
    FDelegateHandle DelegateHandle;
	FTimerHandle TimerHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitReleaseAtDuration* CreateWaitInputAtDuration(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration);
	UFUNCTION()
	void OnReleaseCallback();
	bool RemoveEventAndClearTimer();
	UFUNCTION()
	void OnAtDurationEndFunction();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDurationReached();
	
};
