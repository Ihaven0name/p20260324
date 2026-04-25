#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/AbilityBase_Attack.h"
#include "Ability_CrouchCombo.generated.h"

UCLASS()
class P20260324_API UAbility_CrouchCombo : public UAbilityBase_Attack
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
