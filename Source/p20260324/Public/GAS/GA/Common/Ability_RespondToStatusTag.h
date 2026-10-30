// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "Ability_RespondToStatusTag.generated.h"

UCLASS()
class P20260324_API UAbility_RespondToStatusTag : public UGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAbility_RespondToStatusTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	FGameplayTagContainer StatusTagsToRespondTo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status|Animation")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> StatusMontages;

	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
	void OnStatusTagAdded(FGameplayTag StatusTag);

	UFUNCTION(BlueprintImplementableEvent, Category = "Status")
	void OnStatusTagRemoved(FGameplayTag StatusTag);

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
	void HandleStatusTagCountChanged(const FGameplayTag Tag, int32 NewCount);
	void PlayStatusMontage(const FGameplayTag StatusTag);
	void StopStatusMontage(const FGameplayTag StatusTag);

	TWeakObjectPtr<UAbilitySystemComponent> ListeningAbilitySystemComponent;
	TMap<FGameplayTag, FDelegateHandle> StatusTagDelegateHandles;
};
