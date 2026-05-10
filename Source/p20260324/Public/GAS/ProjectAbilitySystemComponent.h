#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "ProjectAbilitySystemComponent.generated.h"


UCLASS()
class P20260324_API UProjectAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	/**
	 * 从 InputInfo 读取配置并 Give 所有 Ability（仅在 Authority 调用）
	 */
	UFUNCTION(BlueprintCallable)
	void GiveAbilitiesFromInputInfo();

	/**
	 * 通过 GameplayTag 触发技能按下
	 * @param AbilityTag 技能的 Tag
	 */
	UFUNCTION(BlueprintCallable)
	void AbilityInputTagPressed(const FGameplayTag AbilityTag);

	/**
	 * 通过 GameplayTag 触发技能释放
	 * @param AbilityTag 技能的 Tag
	 */
	UFUNCTION(BlueprintCallable)
	void AbilityInputTagReleased(const FGameplayTag AbilityTag);

	/**
	 * 通过 GameplayTag 触发技能按住（持续触发）
	 * @param AbilityTag 技能的 Tag
	 */
	UFUNCTION(BlueprintCallable)
	void AbilityInputTagHeld(const FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable)
	void Print();

	/**
	 * 根据AbilityTag查找对应的AbilitySpec
	 * @param AbilityTag 要查找的AbilityTag
	 * @param OutSpec 找到的AbilitySpec
	 * @return 是否找到
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool GetAbilitySpecByAbilityTag(const FGameplayTag AbilityTag, FGameplayAbilitySpec& OutSpec);

	/**
	 * 通过 Ability 类程序化激活（不依赖 InputPressed 标记）
	 * 用于 Judge Ability 判断完成后激活目标 Ability
	 * @param JudgeInputTag Judge Ability 自身的 AbilityTag（与输入绑定的标签一致）
	 * @return 是否成功尝试激活
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool AbilityJudgeTagPressed(const FGameplayTag JudgeInputTag);

	/**
	 * 通过 JudgeAbility 的 InputAction Tag 激活对应的 Judge Ability
	 * @param JudgeInputTag Judge Ability 自身的 AbilityTag（与输入绑定的标签一致）
	 * @return 是否成功尝试激活
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool AbilityJudgeTagReleased(const FGameplayTag JudgeInputTag);
};

inline void UProjectAbilitySystemComponent::Print()
{
	for (const FGameplayTag& Tag : GameplayTagCountContainer.GetExplicitGameplayTags())
	{
		UE_LOG(LogTemp, Warning, TEXT("  Tag: %s"), *Tag.ToString());
	}
}
