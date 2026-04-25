#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitInputAtDuration.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAtDurationEndSignature);



UCLASS()
class P20260324_API UAbilityTask_WaitInputAtDuration : public UAbilityTask
{
	GENERATED_BODY()
public:
	virtual void Activate() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAtDurationEndSignature FOnAtDurationEndSignature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TaskDuration;
	
	UPROPERTY()
	bool bIsReleased=false;
	UPROPERTY()
	float StartTime=0.f;
	UPROPERTY()
	bool bTestInitialState;
	
    FDelegateHandle DelegateHandle;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitInputAtDuration* CreateWaitInputAtDuration(UGameplayAbility* OwningAbility, FName TaskInstanceName, float Duration);
	UFUNCTION()
	void OnReleaseCallback();
	UFUNCTION()
	void OnAtDurationEndFunction();
	
};
