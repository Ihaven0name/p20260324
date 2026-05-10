#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Info/AbilityInfo.h"
#include "Manager/ConfigManager.h"
#include "GameplayAbilityBase.generated.h"

//在蓝图配置Tags的时候，只需要配置AbilityTag就可以了
UCLASS()
class P20260324_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbilityBase();
	/** 攻击蒙太奇 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<UAnimMontage> AbilityMontage;
	/** 攻击音效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TObjectPtr<USoundBase> AbilitySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	FName AbilityTagName;

	/**
	 * Activation Required Tags (OR 模式)
	 * 只要角色拥有其中任意一个 Tag 就满足激活条件
	 * 用于"多方向/多条件都能触发同一技能"的场景
	 * 留空则不进行 OR 检查（走引擎默认的 AND 逻辑）
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActivationHasAnyTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer InputWindowTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer OwnedTagInMontage;
	
	
	/**
	 * 从 DataTable 初始化标签配置（Base 构造函数自动调用）
	 * @return true 初始化成功 / false 找不到对应配置
	 */
	bool InitializeTags();

	UFUNCTION(BlueprintCallable)
	void RemoveMontageTagAndEndAbility();

protected:
	/** 重写：在父类检查之后，追加 OR 条件标签判断 */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags,
		const FGameplayTagContainer* TargetTags,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr
	) const override;
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
};
