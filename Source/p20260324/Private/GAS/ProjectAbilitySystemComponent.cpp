#include "GAS/ProjectAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Info/InputInfo.h"
#include "Manager/ConfigManager.h"

void UProjectAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

void UProjectAbilitySystemComponent::GiveAbilitiesFromInputInfo()
{
	UConfigManager* ConfigManager = GetWorld()->GetGameInstance()->GetSubsystem<UConfigManager>();
	if (!ConfigManager || !ConfigManager->GetInputInfo()) return;

	for (const TTuple<EPlayerCategory, FInputSetting>& TempMap : ConfigManager->GetInputInfo()->PlayerCategoryToInputSetting)
	{
		const TArray<FInputActionAndAbilityInputData>& IAAndAIDArray = TempMap.Value.InputActionAndAbilityInputDataArray;
		for (const FInputActionAndAbilityInputData& IAAndAID : IAAndAIDArray)
		{
			// Give 目标 Ability（原有逻辑）
			for (const FAbilityInputData& AbilityInputData:IAAndAID.AbilityInputData)
			{
				if (AbilityInputData.GameplayAbilityClass)
				{
					GiveAbility(FGameplayAbilitySpec(AbilityInputData.GameplayAbilityClass, 1, INDEX_NONE, GetAvatarActor()));
				}
			}
			
			// Give Judge Ability（新增）
			if (IAAndAID.JudgeAbility)
			{
				GiveAbility(FGameplayAbilitySpec(IAAndAID.JudgeAbility, 1, INDEX_NONE, GetAvatarActor()));
			}
		}
	}
}

void UProjectAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid()) return;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& TempGameplaySpec : GetActivatableAbilities())
	{
		UGameplayAbility* Temp = TempGameplaySpec.GetPrimaryInstance();
		if (Temp->AbilityTags.HasTagExact(AbilityTag))
		{
			AbilitySpecInputPressed(TempGameplaySpec);
			// 触发网络复制事件
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, 
				TempGameplaySpec.Handle, 
				TempGameplaySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UProjectAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid()) return;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& TempGameplaySpec : GetActivatableAbilities())
	{
		if (TempGameplaySpec.GetPrimaryInstance()->AbilityTags.HasTagExact(AbilityTag))
		{
			AbilitySpecInputReleased(TempGameplaySpec);
			// 触发网络复制事件
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, 
				TempGameplaySpec.Handle, 
				TempGameplaySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UProjectAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag AbilityTag)
{
	if (!AbilityTag.IsValid()) return;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& TempGameplaySpec : GetActivatableAbilities())
	{
		if (TempGameplaySpec.GetPrimaryInstance()->AbilityTags.HasTagExact(AbilityTag))
		{
			// 按住时触发输入但不标记为 Pressed（用于持续触发技能）
			AbilitySpecInputPressed(TempGameplaySpec);
			TryActivateAbility(TempGameplaySpec.Handle);
		}
	}
}
//只需要配置AbilityTag就可以
bool UProjectAbilitySystemComponent::GetAbilitySpecByAbilityTag(const FGameplayTag AbilityTag, FGameplayAbilitySpec& OutSpec)
{
	if (!AbilityTag.IsValid()) return false;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.GetPrimaryInstance()->AbilityTags.HasTagExact(AbilityTag))
		{
			OutSpec = Spec;
			return true;
		}
	}
	return false;
}

bool UProjectAbilitySystemComponent::AbilityJudgeTagPressed(const FGameplayTag JudgeInputTag)
{
	if (!JudgeInputTag.IsValid()) return false;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& TempGameplaySpec : GetActivatableAbilities())
	{
		if (TempGameplaySpec.GetPrimaryInstance()->AbilityTags.HasTagExact(JudgeInputTag))
		{
			AbilitySpecInputPressed(TempGameplaySpec);
			// 触发网络复制事件
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, 
				TempGameplaySpec.Handle, 
				TempGameplaySpec.ActivationInfo.GetActivationPredictionKey());
			TryActivateAbility(TempGameplaySpec.Handle);
		}
	}
	return false;
}

bool UProjectAbilitySystemComponent::AbilityJudgeTagReleased(const FGameplayTag JudgeInputTag)
{
	if (!JudgeInputTag.IsValid()) return false;

	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& TempGameplaySpec : GetActivatableAbilities())
	{
		if (TempGameplaySpec.GetPrimaryInstance()->AbilityTags.HasTagExact(JudgeInputTag))
		{
			AbilitySpecInputReleased(TempGameplaySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, 
				TempGameplaySpec.Handle, 
				TempGameplaySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
	return false;
}

