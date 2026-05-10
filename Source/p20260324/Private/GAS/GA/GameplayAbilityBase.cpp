#include "GAS/GA/GameplayAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Manager/AbilityTagManager.h"


UGameplayAbilityBase::UGameplayAbilityBase()
{
	
}

bool UGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// 先走父类所有默认检查（冷却、消耗、AND 标签等）
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// OR 条件检查：只要角色拥有 ActivationRequiredTags_Any 中任意一个 Tag 就通过
	if (ActivationHasAnyTags.Num() > 0 && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayTagContainer OwnedTags;
		ActorInfo->AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);

		bool bHasAny = false;
		for (const FGameplayTag& ReqTag : ActivationHasAnyTags)
		{
			if (OwnedTags.HasTagExact(ReqTag))
			{
				bHasAny = true;
				
				break;
			}
		}
		if (!bHasAny)
		{
			return false;
		}
	}
	return true;
}
void UGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}



void UGameplayAbilityBase::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),InputWindowTag,true);
}


bool UGameplayAbilityBase::InitializeTags()
{
	
	const FAbilityTagConfig* Config = UAbilityTagManager::FindAbilityTagConfigByRowName(AbilityTagName);
	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to find AbilityTagConfig for RowName: %s"), *GetName(), *AbilityTagName.ToString());
		return false;
	}

	ActivationHasAnyTags = Config->ActivationHasAnyTags;
	InputWindowTag = Config->InputWindowTag;
	OwnedTagInMontage = Config->OwnedTagInMontage;
	AbilityTags = Config->AbilityTags;
	CancelAbilitiesWithTag = Config->CancelAbilitiesWithTag;
	BlockAbilitiesWithTag = Config->BlockAbilitiesWithTag;
	ActivationOwnedTags = Config->ActivationOwnedTags;
	ActivationRequiredTags = Config->ActivationRequiredTags;
	ActivationBlockedTags = Config->ActivationBlockedTags;
	SourceRequiredTags = Config->SourceRequiredTags;
	SourceBlockedTags = Config->SourceBlockedTags;
	TargetRequiredTags = Config->TargetRequiredTags;
	TargetBlockedTags = Config->TargetBlockedTags;

	return true;
}

void UGameplayAbilityBase::RemoveMontageTagAndEndAbility()
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),OwnedTagInMontage,true);
	K2_EndAbility();
}

