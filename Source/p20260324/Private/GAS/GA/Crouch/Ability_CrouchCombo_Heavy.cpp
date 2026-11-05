#include "GAS/GA/Crouch/Ability_CrouchCombo_Heavy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/Character.h"
#include "Interface/PlayerInterface.h"

void UAbility_CrouchCombo_Heavy::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayTagContainer HeldWindowTags;
			for (const FGameplayTag& WindowTag : InputWindowTag)
			{
				if (ASC->GetTagCount(WindowTag) > 0)
				{
					HeldWindowTags.AddTag(WindowTag);
				}
			}
			if (!HeldWindowTags.IsEmpty())
			{
				UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AvatarActor, HeldWindowTags, AvatarActor->HasAuthority());
			}
		}
	}
	CloseToTarget();
	PlayAttackMontage();
}

void UAbility_CrouchCombo_Heavy::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
