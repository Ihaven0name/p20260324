// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/Common/Ability_RespondToStatusTag.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

UAbility_RespondToStatusTag::UAbility_RespondToStatusTag()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UAbility_RespondToStatusTag::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!IsValid(ASC))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ListeningAbilitySystemComponent = ASC;
	for (const FGameplayTag& StatusTag : StatusTagsToRespondTo)
	{
		if (!StatusTag.IsValid())
		{
			continue;
		}

		FDelegateHandle DelegateHandle = ASC->RegisterGameplayTagEvent(
			StatusTag,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
				this,
				&ThisClass::HandleStatusTagCountChanged);
		StatusTagDelegateHandles.Add(StatusTag, DelegateHandle);

		const int32 ExistingCount = ASC->GetTagCount(StatusTag);
		if (ExistingCount > 0)
		{
			HandleStatusTagCountChanged(StatusTag, ExistingCount);
		}
	}
}

void UAbility_RespondToStatusTag::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	if (UAbilitySystemComponent* ASC = ListeningAbilitySystemComponent.Get())
	{
		if (UAnimMontage* ActiveMontage = ASC->GetCurrentMontage())
		{
			for (const TPair<FGameplayTag, TObjectPtr<UAnimMontage>>& Entry : StatusMontages)
			{
				if (Entry.Value == ActiveMontage)
				{
					ASC->CurrentMontageStop(0.15f);
					break;
				}
			}
		}

		for (const TPair<FGameplayTag, FDelegateHandle>& Entry : StatusTagDelegateHandles)
		{
			ASC->RegisterGameplayTagEvent(Entry.Key, EGameplayTagEventType::NewOrRemoved).Remove(Entry.Value);
		}
	}

	StatusTagDelegateHandles.Reset();
	ListeningAbilitySystemComponent.Reset();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_RespondToStatusTag::HandleStatusTagCountChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnStatusTagAdded(Tag);
		PlayStatusMontage(Tag);
	}
	else
	{
		OnStatusTagRemoved(Tag);
		StopStatusMontage(Tag);
	}
}

void UAbility_RespondToStatusTag::PlayStatusMontage(const FGameplayTag StatusTag)
{
	const TObjectPtr<UAnimMontage>* Montage = StatusMontages.Find(StatusTag);
	UAbilitySystemComponent* ASC = ListeningAbilitySystemComponent.Get();
	if (!Montage || !IsValid(*Montage) || !IsValid(ASC) || ASC->GetCurrentMontage() == *Montage)
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		FName(*FString::Printf(TEXT("Status_%s"), *StatusTag.ToString().Replace(TEXT("."), TEXT("_")))),
		*Montage);
	if (MontageTask)
	{
		MontageTask->ReadyForActivation();
	}
}

void UAbility_RespondToStatusTag::StopStatusMontage(const FGameplayTag StatusTag)
{
	const TObjectPtr<UAnimMontage>* Montage = StatusMontages.Find(StatusTag);
	UAbilitySystemComponent* ASC = ListeningAbilitySystemComponent.Get();
	if (!Montage || !IsValid(*Montage) || !IsValid(ASC) || ASC->GetCurrentMontage() != *Montage)
	{
		return;
	}

	for (const TPair<FGameplayTag, TObjectPtr<UAnimMontage>>& Entry : StatusMontages)
	{
		if (Entry.Key != StatusTag && Entry.Value == *Montage && ASC->GetTagCount(Entry.Key) > 0)
		{
			return;
		}
	}

	ASC->CurrentMontageStop(0.15f);
}

