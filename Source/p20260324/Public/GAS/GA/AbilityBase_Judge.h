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

protected:
	// ==========================================
	// 蓄力判断相关变量（与 Ability_CrouchCombo 一致）
	// ==========================================
	
	/** 是否启用蓄力阈值检测 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	bool bOpenPressThreshold = true;

	/** true=短按触发目标Ability / false=长按触发目标Ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	bool bIsShortPress = true;

	/** 蓄力判定阈值（秒），超过此时间为长按，否则为短按 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	float PressThreshold = 0.07f;

	// ==========================================
	// TargetAbility 映射
	// ==========================================

	/** 短按时要激活的目标 Ability 类 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	TSubclassOf<UGameplayAbility> ShortPressTargetAbility;

	/** 长按时要激活的目标 Ability 类 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
	TSubclassOf<UGameplayAbility> LongPressTargetAbility;

protected:
	/** 启动蓄力等待 Task */
	UFUNCTION(BlueprintCallable)
	void WaitReleaseAtDuration();

	/** 短按回调：在 Duration 之前松手 → bReleaseAtDuration=true → 激活 ShortPressTargetAbility 或 LongPressTargetAbility */
	UFUNCTION(BlueprintCallable)
	void OnAtDurationEndForShortPress(const bool bReleaseAtDuration);

	/** 长按回调：到达 Duration 还没松手 → bReleaseAtDuration=false → 激活对应 TargetAbility */
	UFUNCTION(BlueprintCallable)
	void OnAtDurationEndForLongPress(const bool bReleaseAtDuration);

	/** 根据当前配置获取应该激活的目标 Ability 类 */
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UGameplayAbility> GetTargetAbilityClass() const;

private:
	/** 当前正在运行的蓄力等待 Task 引用（用于 End 时清理） */
	UAbilityTask_WaitReleaseAtDuration* CurrentWaitReleaseTask = nullptr;
};
