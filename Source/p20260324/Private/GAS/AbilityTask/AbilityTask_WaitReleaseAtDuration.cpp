#include "GAS/AbilityTask/AbilityTask_WaitReleaseAtDuration.h"

#include "AbilitySystemComponent.h"

void UAbilityTask_WaitReleaseAtDuration::Activate()
{
	StartTime = GetWorld()->GetTimeSeconds();
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		if (bTestInitialState && IsLocallyControlled())
		{
			FGameplayAbilitySpec *Spec = Ability->GetCurrentAbilitySpec();
			if (Spec && !Spec->InputPressed)
			{
				OnReleaseCallback();
				return;
			}
		}

		DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UAbilityTask_WaitReleaseAtDuration::OnReleaseCallback);
		if (IsForRemoteClient())
		{
			if (!ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()))
			{
				SetWaitingOnRemotePlayerData();
			}
		}
		//客户端和服务器都设置一个Timer，客户端进行预测，服务器进行验证
  		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ThisClass::OnAtDurationEndFunction,
			TaskDuration
		);
	}
}

UAbilityTask_WaitReleaseAtDuration* UAbilityTask_WaitReleaseAtDuration::CreateWaitInputAtDuration(
	UGameplayAbility* OwningAbility,
	const FName TaskInstanceName,
	const float Duration)
{
	UAbilityTask_WaitReleaseAtDuration* TaskInstance = NewAbilityTask<UAbilityTask_WaitReleaseAtDuration>(OwningAbility,TaskInstanceName);
	TaskInstance->TaskDuration = Duration;
	return TaskInstance;
}

void UAbilityTask_WaitReleaseAtDuration::OnReleaseCallback()
{
	if (!RemoveEventAndClearTimer()) return;
	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (IsPredictingClient())
	{
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}
	if (Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s:Time:%f"),*Ability->GetClass()->GetName(), ElapsedTime);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("wei sha a"));
	}
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAtDurationEndSignature.Broadcast(true);
	}
	EndTask();
}



void UAbilityTask_WaitReleaseAtDuration::OnAtDurationEndFunction()
{
	if (!RemoveEventAndClearTimer()) return;
	if (Ability->HasAuthority(&Ability->GetCurrentActivationInfoRef()))
	{
		Multicast_OnDurationReached();
	}
}

void UAbilityTask_WaitReleaseAtDuration::Multicast_OnDurationReached_Implementation()
{
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// false 表示是由计时器触发（长按），而不是由松手触发（短按）
		OnAtDurationEndSignature.Broadcast(false);
		EndTask();
	}
}

bool UAbilityTask_WaitReleaseAtDuration::RemoveEventAndClearTimer()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!Ability || !ASC)
	{
		return false;
	}
	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);
	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	return true;
}
