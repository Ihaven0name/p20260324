#include "GAS/AbilityTask/AbilityTask_WaitInputAtDuration.h"

#include "AbilitySystemComponent.h"

void UAbilityTask_WaitInputAtDuration::Activate()
{
	Super::Activate();
	if (Ability->GetCurrentActorInfo()->PlayerController.Get()->IsLocalController())
	{
		
	}
	else
	{
		const FGameplayAbilitySpecHandle& TempSpecHandle=GetAbilitySpecHandle();
		const FPredictionKey& TempKey=GetActivationPredictionKey(); 
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(TempSpecHandle,TempKey).AddUObject(this,&UAbilityTask_SpawnForwardCursor::AbilityTargetDataSetFunction);
		const bool bCalledDelegate= AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(TempSpecHandle,TempKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

UAbilityTask_WaitInputAtDuration* UAbilityTask_WaitInputAtDuration::CreateWaitInputAtDuration(
	UGameplayAbility* OwningAbility,
	const FName TaskInstanceName,
	const float Duration)
{
	UAbilityTask_WaitInputAtDuration* TaskInstance = NewObject<UAbilityTask_WaitInputAtDuration>(OwningAbility,TaskInstanceName);
	TaskInstance->TaskDuration = Duration;
	return TaskInstance;
}

void UAbilityTask_WaitInputAtDuration::OnReleaseCallback()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;

	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!Ability || !ASC)
	{
		return;
	}

	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	if (IsPredictingClient())
	{
		// Tell the server about this
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	// We are done. Kill us so we don't keep getting broadcast messages
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnRelease.Broadcast(ElapsedTime);
	}
	EndTask();
}

void UAbilityTask_WaitInputAtDuration::OnAtDurationEndFunction()
{
	
}
