#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/AbilityBase_Attack.h"
#include "Ability_CrouchCombo.generated.h"

UCLASS()
class P20260324_API UAbility_CrouchCombo : public UAbilityBase_Attack
{
	GENERATED_BODY()
public:
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOpenChase=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PrimaryChaseDistance=20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockingChaseDistance=30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinKeepDistance=10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseDuration=0.1f;
	

	//用来实现释放连招蒙太奇的时候进行向前的一定长度的移动，在非Lock状态的时候，默认向玩家Forward方向进行较短的移动，在Lock状态的时候，向目标方向进行相对较远的移动
	UFUNCTION(BlueprintCallable)
	void CloseToTarget();
	UFUNCTION(BlueprintCallable)
	void PlayAttackMontage();
};
