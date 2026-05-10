#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/Crouch/Ability_CrouchCombo.h"
#include "Ability_CrouchCombo_Heavy.generated.h"


UCLASS()
class P20260324_API UAbility_CrouchCombo_Heavy : public UAbility_CrouchCombo
{
	GENERATED_BODY()
public:
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
