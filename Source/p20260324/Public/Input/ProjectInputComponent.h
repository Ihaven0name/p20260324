// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Info/InputInfo.h"
#include "ProjectInputComponent.generated.h"

class UInputInfo;
class UInputAction;

UCLASS()
class P20260324_API UProjectInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	/**
	 * 绑定所有角色分类的所有技能输入 IA 到统一回调（Started + Completed）
	 * 遍历 InputInfo 中每个 EPlayerCategory 的 FInputSetting，
	 * 对其中每个 IA 的每个 AbilityTag 绑定 Started/Completed 事件
	 * @param InputInfo 输入配置数据资产
	 * @param User 回调函数所属对象
	 * @param PressFunc 按下(Started)回调，签名 void(UserClass*, FGameplayTag)
	 * @param ReleaseFunc 松开(Completed)回调，签名 void(UserClass*, FGameplayTag)
	 */
	template<class UserClass, typename PressFuncType, typename ReleaseFuncType>
	void BindAbilityActions(const UInputInfo* InputInfo, UserClass* User,
		PressFuncType PressFunc, ReleaseFuncType ReleaseFunc);

	/**
	 * 绑定所有角色分类的 Judge Ability 输入（Started + Completed）
	 * 遍历 InputInfo 中每个 FInputActionAndAbilityInputData，
	 * 取其 JudgeTag（Ability.Judge.* 标签）绑定到 Judge 回调
	 * @param InputInfo 输入配置数据资产
	 * @param User 回调函数所属对象
	 * @param PressFunc 按下(Started)回调，签名 void(UserClass*, FGameplayTag) — 用于激活 JudgeAbility
	 * @param ReleaseFunc 松开(Completed)回调，签名 void(UserClass*, FGameplayTag) — 用于通知 ASC Released
	 */
	template<class UserClass, typename PressFuncType, typename ReleaseFuncType>
	void BindJudgeActions(const UInputInfo* InputInfo, UserClass* User,
		PressFuncType PressFunc, ReleaseFuncType ReleaseFunc);
};

template <class UserClass, typename PressFuncType, typename ReleaseFuncType>
void UProjectInputComponent::BindAbilityActions(const UInputInfo* InputInfo, UserClass* User,
	PressFuncType PressFunc, ReleaseFuncType ReleaseFunc)
{
	check(InputInfo);
	for (const TTuple<EPlayerCategory, FInputSetting>& Pair : InputInfo->PlayerCategoryToInputSetting)
	{
		const FInputSetting& Setting = Pair.Value;
		for (const FInputActionAndAbilityInputData& IAData : Setting.InputActionAndAbilityInputDataArray)
		{
			if (!IAData.InputAction) continue;
			for (const FAbilityInputData& AbilityData : IAData.AbilityInputData)
			{
				if (!AbilityData.AbilityTag.IsValid()) continue;
				if (PressFunc)
				{
					BindAction(IAData.InputAction, ETriggerEvent::Started,
						User, PressFunc, AbilityData.AbilityTag);
				}
				if (ReleaseFunc)
				{
					BindAction(IAData.InputAction, ETriggerEvent::Completed,
						User, ReleaseFunc, AbilityData.AbilityTag);
				}
			}
		}
	}
}

template <class UserClass, typename PressFuncType, typename ReleaseFuncType>
void UProjectInputComponent::BindJudgeActions(const UInputInfo* InputInfo, UserClass* User,
	PressFuncType PressFunc, ReleaseFuncType ReleaseFunc)
{
	check(InputInfo);
	for (const TTuple<EPlayerCategory, FInputSetting>& Pair : InputInfo->PlayerCategoryToInputSetting)
	{
		const FInputSetting& Setting = Pair.Value;
		for (const FInputActionAndAbilityInputData& IAData : Setting.InputActionAndAbilityInputDataArray)
		{
			// 直接使用配置的 JudgeTag（Ability.Judge.* 标签）
			if (!IAData.InputAction || !IAData.JudgeTag.IsValid()) continue;

			// 绑定 Started → OnJudgeInputStarted（激活 JudgeAbility）
			if (PressFunc)
			{
				BindAction(IAData.InputAction, ETriggerEvent::Started,
					User, PressFunc, IAData.JudgeTag);
			}

			// 绑定 Completed → OnJudgeInputReleased（通知 ASC Released）
			if (ReleaseFunc)
			{
				BindAction(IAData.InputAction, ETriggerEvent::Completed,
					User, ReleaseFunc, IAData.JudgeTag);
			}
		}
	}
}
