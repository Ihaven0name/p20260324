#include "GAS/GA/GameplayAbilityBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
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
	FGameplayTagContainer OwnedTags;
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
	}

	// Animation input windows decide when the owning client may request the next combo step.
	// The server validates the combo state below, but must not reject a request just because
	// its RPC arrived after the server-side animation notify closed.
	if (ActorInfo && ActorInfo->IsLocallyControlled() && LocalInputWindowRequiredTags.Num() > 0
		&& ActorInfo->AbilitySystemComponent.IsValid()
		&& !ActorInfo->AbilitySystemComponent->HasAllMatchingGameplayTags(LocalInputWindowRequiredTags))
	{
		return false;
	}

	// 先走父类所有默认检查（冷却、消耗、AND 标签等）
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	// OR 条件检查：只要角色拥有 ActivationRequiredTags_Any 中任意一个 Tag 就通过
	if (ActivationHasAnyTags.Num() > 0 && ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
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

	FGameplayTagContainer TagsToConsume = InputWindowTag;
	for (const FGameplayTag& WindowTag : LocalInputWindowRequiredTags)
	{
		TagsToConsume.RemoveTag(WindowTag);
	}
	FGameplayTagContainer HeldTagsToConsume;
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
	{
		for (const FGameplayTag& WindowTag : TagsToConsume)
		{
			if (ASC->GetTagCount(WindowTag) > 0)
			{
				HeldTagsToConsume.AddTag(WindowTag);
			}
		}
	}
	if (!HeldTagsToConsume.IsEmpty())
	{
		AActor* AvatarActor = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(
			AvatarActor,
			HeldTagsToConsume,
			AvatarActor && AvatarActor->HasAuthority());
	}
}


bool UGameplayAbilityBase::InitializeTags()
{
	const FAbilityTagConfig* Config = FAbilityTagManager::FindAbilityTagConfigByRowName(AbilityTagName);
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
	LocalInputWindowRequiredTags.Reset();
	for (const FGameplayTag& WindowTag : InputWindowTag)
	{
		if (ActivationRequiredTags.HasTagExact(WindowTag))
		{
			LocalInputWindowRequiredTags.AddTag(WindowTag);
			ActivationRequiredTags.RemoveTag(WindowTag);
		}
	}
	ActivationBlockedTags = Config->ActivationBlockedTags;
	SourceRequiredTags = Config->SourceRequiredTags;
	SourceBlockedTags = Config->SourceBlockedTags;
	TargetRequiredTags = Config->TargetRequiredTags;
	TargetBlockedTags = Config->TargetBlockedTags;

	return true;
}

void UGameplayAbilityBase::RemoveMontageTagAndEndAbility()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(
		AvatarActor,
		OwnedTagInMontage,
		AvatarActor && AvatarActor->HasAuthority());
	K2_EndAbility();
}

