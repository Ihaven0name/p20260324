#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "GAS/AbilityTask/AbilityTask_WaitReleaseAtDuration.h"
#include "AbilityBase_Judge.generated.h"

/**
 * Judge Ability 基类
 * 职责：绑定 InputAction，通过 AbilityTask_WaitReleaseAtDuration 判断短按/长按，
 *       然后程序化激活对应的 TargetAbility
 * 每个 InputAction 对应一个 Judge 实例
 */
UCLASS()
class P20260324_API UAbilityBase_Judge : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UAbilityBase_Judge();

protected:
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
	virtual void OnGiveAbility(
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilitySpec& Spec
	) override;
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void TryActivateAbilityFromJudge(const bool bIsShortPress);

public:
	
	// ==========================================
	// 蓄力判断相关变量（与 Ability_CrouchCombo 一致）
	// ==========================================
	
	/** 是否启用蓄力阈值检测 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	bool bOpenPressThreshold = true;

	/** 蓄力判定阈值（秒），超过此时间为长按，否则为短按 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float PressThreshold = 0.07f;

	// ==========================================
	// TargetAbility 映射
	// ==========================================

	/** 短按时要激活的目标 Ability 类 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	TArray<FAbilityInputData> ShortPressInputData;

	/** 长按时要激活的目标 Ability 类 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	TArray<FAbilityInputData> LongPressInputData;

	UPROPERTY()
	uint8 ShortPressIndex=0;
	UPROPERTY()
	uint8 ShortPressLength=0;
	UPROPERTY()
	uint8 LongPressIndex=0;
	UPROPERTY()
	uint8 LongPressLength=0;

protected:
	/** 启动蓄力等待 Task */
	UFUNCTION(BlueprintCallable)
	void WaitReleaseAtDuration();

	
	UFUNCTION(BlueprintCallable)
	void OnAtDurationEndPress(const bool bReleaseAtDuration);

	
};
