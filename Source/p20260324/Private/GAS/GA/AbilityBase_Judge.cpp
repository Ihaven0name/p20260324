#include "GAS/GA/AbilityBase_Judge.h"

#include "AbilitySystemComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"

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

void UAbilityBase_Judge::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	ShortPressLength=ShortPressInputData.Num();
	LongPressLength=LongPressInputData.Num();
}

void UAbilityBase_Judge::TryActivateAbilityFromJudge(const bool bIsShortPress)
{
	UAbilitySystemComponent* ASC=GetAbilitySystemComponentFromActorInfo();
	FGameplayAbilitySpec Spec;
	if (bIsShortPress)
	{
		for (uint8 i=0; i<ShortPressLength; i++)
		{
			if (UProjectBlueprintFunctionLibrary::CanActivateAbilityAndOutputSpec(ASC,ShortPressInputData[ShortPressIndex].AbilityTag,Spec))
			{
				ASC->TryActivateAbility(Spec.Handle);
				ShortPressIndex+=1;
				if (ShortPressIndex==ShortPressLength)
				{
					ShortPressIndex=0;
				}
				break;
			}
			ShortPressIndex+=1;
			if (ShortPressIndex==ShortPressLength)
			{
				ShortPressIndex=0;
			}
		}
	}
	else
	{
		for (uint8 i=0; i<LongPressLength; i++)
		{
			if (UProjectBlueprintFunctionLibrary::CanActivateAbilityAndOutputSpec(ASC,LongPressInputData[LongPressIndex].AbilityTag,Spec))
			{
				ASC->TryActivateAbility(Spec.Handle);
				LongPressIndex+=1;
				if (LongPressIndex==LongPressLength)
				{
					LongPressIndex=0;
				}
				break;
			}
			LongPressIndex+=1;
			if (LongPressIndex==LongPressLength)
			{
				LongPressIndex=0;
			}
		}
	}
	
}

void UAbilityBase_Judge::WaitReleaseAtDuration()
{
	if (!bOpenPressThreshold)
	{
		//不开始使用按压阈值，默认是短按技能
		TryActivateAbilityFromJudge(true);
		K2_EndAbility();
		return;
	}

	// 创建蓄力等待 Task
	UAbilityTask_WaitReleaseAtDuration* CurrentWaitReleaseTask = UAbilityTask_WaitReleaseAtDuration::CreateWaitInputAtDuration(
		this,
		TEXT("JudgeWaitReleaseAtDuration"),
		PressThreshold
	);

	CurrentWaitReleaseTask->OnAtDurationEndSignature.AddDynamic(this, &ThisClass::OnAtDurationEndPress);
	CurrentWaitReleaseTask->ReadyForActivation();
}

void UAbilityBase_Judge::OnAtDurationEndPress(const bool bReleaseAtDuration)
{
	TryActivateAbilityFromJudge(bReleaseAtDuration);
	// 无论是否成功激活目标 Ability，Judge 本身都结束
	K2_EndAbility();
}