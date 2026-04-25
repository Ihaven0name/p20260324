#include "GAS/GA/AbilityBase_Judge.h"

#include "AbilitySystemComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UAbilityBase_Judge::UAbilityBase_Judge()
{
	
}

void UAbilityBase_Judge::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// 启动蓄力判断流程
	WaitReleaseAtDuration();
}

void UAbilityBase_Judge::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

TSubclassOf<UGameplayAbility> UAbilityBase_Judge::GetTargetAbilityClass() const
{
	if (bIsShortPress)
	{
		
	}
	return LongPressTargetAbility;
}

void UAbilityBase_Judge::WaitReleaseAtDuration()
{
	if (!bOpenPressThreshold)
	{
		// 不启用蓄力检测，直接激活目标 Ability 并结束
		TSubclassOf<UGameplayAbility> TargetClass = GetTargetAbilityClass();
		if (TargetClass)
		{
			UProjectAbilitySystemComponent* ProjectASC = Cast<UProjectAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
			if (ProjectASC)
			{
				ProjectASC->AbilityJudgeTagPressed(TargetClass);
			}
		}
		K2_EndAbility();
		return;
	}

	// 创建蓄力等待 Task
	CurrentWaitReleaseTask = UAbilityTask_WaitReleaseAtDuration::CreateWaitInputAtDuration(
		this,
		TEXT("JudgeWaitReleaseAtDuration"),
		PressThreshold
	);

	if (bIsShortPress)
	{
		CurrentWaitReleaseTask->OnAtDurationEndSignature.AddDynamic(this, &ThisClass::OnAtDurationEndForShortPress);
	}
	else
	{
		CurrentWaitReleaseTask->OnAtDurationEndSignature.AddDynamic(this, &ThisClass::OnAtDurationEndForLongPress);
	}

	CurrentWaitReleaseTask->ReadyForActivation();
}

void UAbilityBase_Judge::OnAtDurationEndForShortPress(const bool bReleaseAtDuration)
{
	/*
	 * 短按模式：
	 * - bReleaseAtDuration == true  → 在 Duration 前松手了（短按） → 激活目标
	 * - bReleaseAtDuration == false → 到达 Duration 还没松手（长按）→ 结束不激活
	 */
	if (bReleaseAtDuration)
	{
		// 短按：松手了，激活目标 Ability
		TSubclassOf<UGameplayAbility> TargetClass = GetTargetAbilityClass();
		if (TargetClass)
		{
			UProjectAbilitySystemComponent* ProjectASC = Cast<UProjectAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
			if (ProjectASC)
			{
				ProjectASC->AbilityJudgeTagPressed(TargetClass);
			}
		}
	}
	
	// 无论是否成功激活目标 Ability，Judge 本身都结束
	K2_EndAbility();
}

void UAbilityBase_Judge::OnAtDurationEndForLongPress(const bool bReleaseAtDuration)
{
	/*
	 * 长按模式：
	 * - bReleaseAtDuration == true  → 在 Duration 前松手了（短按）→ 结束不激活
	 * - bReleaseAtDuration == false → 到达 Duration 还没松手（长按）→ 激活目标
	 */
	if (!bReleaseAtDuration)
	{
		// 长按：到达 Duration 且未松手，激活目标 Ability
		TSubclassOf<UGameplayAbility> TargetClass = GetTargetAbilityClass();
		if (TargetClass)
		{
			UProjectAbilitySystemComponent* ProjectASC = Cast<UProjectAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
			if (ProjectASC)
			{
				ProjectASC->AbilityJudgeTagPressed(TargetClass);
			}
		}
	}
	
	// 无论是否成功激活目标 Ability，Judge 本身都结束
	K2_EndAbility();
}
