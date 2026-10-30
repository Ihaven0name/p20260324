#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "Interface/CharacterInterface.h"
#include "Ability_HitReact.generated.h"

class ACharacter;
class UBrainComponent;
struct FHitResult;

UCLASS()
class P20260324_API UAbility_HitReact : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAbility_HitReact();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

private:
	void ApplyHitReactControlLock();
	void RemoveHitReactControlLock();
	void FinishHitReact();

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);

	UFUNCTION()
	void OnHitReactMontageEnded();

	bool bHitReactControlLockApplied = false;
	bool bWaitingForLanding = false;
	TWeakObjectPtr<UBrainComponent> PausedBrainComponent;
	TWeakObjectPtr<ACharacter> LandingWaitCharacter;
};
