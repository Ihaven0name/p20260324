#include "GAS/GA/Crouch/Ability_CrouchCombo.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UAbility_CrouchCombo::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_PlayMontageAndWait* AbilityTask_PlayMontageAndWait=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage,
		1.f,
		FName("Default")
	);
	AbilityTask_PlayMontageAndWait->OnInterrupted.AddDynamic(this,&UAbility_CrouchCombo::K2_EndAbility);
	AbilityTask_PlayMontageAndWait->OnCancelled.AddDynamic(this,&UAbility_CrouchCombo::K2_EndAbility);
	AbilityTask_PlayMontageAndWait->OnCompleted.AddDynamic(this,&UAbility_CrouchCombo::K2_EndAbility);
	AbilityTask_PlayMontageAndWait->ReadyForActivation();
}

void UAbility_CrouchCombo::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
