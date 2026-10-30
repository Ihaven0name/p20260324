#include "GAS/GA/Enemy/Wraith/Ability_Shoot.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayEffectType.h"
#include "Interface/SceneActorInterface.h"

UAbility_Shoot::UAbility_Shoot()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UAbility_Shoot::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (!IsValid(AvatarActor) || !AvatarActor->HasAuthority() || !ShootEventTag.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitGameplayEvent* ShootEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		ShootEventTag,
		nullptr,
		true,
		true);
	if (!ShootEventTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	ShootEventTask->EventReceived.AddDynamic(this, &ThisClass::OnShootEventReceived);
	ShootEventTask->ReadyForActivation();

	if (AbilityMontage)
	{
		UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(AvatarActor, OwnedTagInMontage, true);
		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				TEXT("ShootMontageTask"),
				AbilityMontage,
				1.f,
				FName("Default"));

		if (MontageTask)
		{
			MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::RemoveMontageTagAndEndAbility);
			MontageTask->OnCancelled.AddDynamic(this, &ThisClass::RemoveMontageTagAndEndAbility);
			MontageTask->OnCompleted.AddDynamic(this, &ThisClass::RemoveMontageTagAndEndAbility);
			MontageTask->ReadyForActivation();
		}
		else
		{
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AvatarActor, OwnedTagInMontage, true);
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
}

void UAbility_Shoot::OnShootEventReceived(FGameplayEventData Payload)
{
	TraceShotAndNotifyHits();
	if (!AbilityMontage)
	{
		K2_EndAbility();
	}
}

void UAbility_Shoot::TraceShotAndNotifyHits()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor))
	{
		return;
	}

	USkeletalMeshComponent* Mesh = AvatarActor->FindComponentByClass<USkeletalMeshComponent>();
	UWorld* World = AvatarActor->GetWorld();
	if (!World || SocketName.IsNone() || !IsValid(Mesh) || !Mesh->DoesSocketExist(SocketName))
	{
		return;
	}

	const FVector TraceStart = Mesh->GetSocketLocation(SocketName);
	const FVector BaseAimDirection = AvatarActor->GetActorForwardVector().GetSafeNormal();
	if (BaseAimDirection.IsNearlyZero())
	{
		return;
	}

	// 每次收到射击事件时，分别在水平和垂直方向随机偏转一次。
	FRotator ShotRotation = BaseAimDirection.Rotation();
	ShotRotation.Yaw += FMath::FRandRange(-HorizontalSpreadAngle, HorizontalSpreadAngle);
	ShotRotation.Pitch = FMath::Clamp(
		ShotRotation.Pitch + FMath::FRandRange(-VerticalSpreadAngle, VerticalSpreadAngle),
		-89.0f,
		89.0f);
	const FVector ShotDirection = ShotRotation.Vector().GetSafeNormal();

	const FVector TraceEnd = TraceStart + ShotDirection * TraceRange;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(AbilityShoot), true, AvatarActor);
	TArray<FHitResult> TraceHits;
	World->LineTraceMultiByChannel(TraceHits, TraceStart, TraceEnd, TraceChannel, QueryParams);

	if (bDrawDebugTrace)
	{
		const FHitResult* FirstBlockingHit = nullptr;
		bool bHitPlayerBeforeBlocking = false;
		for (const FHitResult& Hit : TraceHits)
		{
			if (const AActor* HitActor = Hit.GetActor(); IsValid(HitActor) && HitActor->ActorHasTag(FName(TEXT("Player"))))
			{
				bHitPlayerBeforeBlocking = true;
			}

			if (Hit.bBlockingHit)
			{
				FirstBlockingHit = &Hit;
				break;
			}
		}

		const FVector DebugEnd = FirstBlockingHit ? FirstBlockingHit->ImpactPoint : TraceEnd;
		const FColor DebugColor = bHitPlayerBeforeBlocking
			? FColor::Green
			: (FirstBlockingHit ? FColor::Red : FColor::Yellow);
		DrawDebugLine(World, TraceStart, DebugEnd, DebugColor, false, DebugTraceDuration, 0, 1.5f);
		if (FirstBlockingHit)
		{
			DrawDebugPoint(World, FirstBlockingHit->ImpactPoint, 8.0f, DebugColor, false, DebugTraceDuration);
		}
	}

	TMap<AActor*, TArray<FHitResult>> HitsByActor;
	AActor* FirstHitActor = nullptr;
	for (const FHitResult& Hit : TraceHits)
	{
		AActor* HitActor = Hit.GetActor();
		if (IsValid(HitActor))
		{
			if (!FirstHitActor)
			{
				FirstHitActor = HitActor;
			}
			HitsByActor.FindOrAdd(HitActor).Add(Hit);
		}

		if (Hit.bBlockingHit)
		{
			break;
		}	
	}

	// 只对射线命中的第一个 Actor 造成伤害，并且要求它带有普通 Actor Tag "Player"。
	if (IsValid(FirstHitActor) && FirstHitActor->ActorHasTag(FName(TEXT("Player"))))
	{
		const FDamageEffectParam DamageParam = GetDefaultDamageEffectParam(FirstHitActor);
		if (DamageParam.DamageEffectClass && DamageParam.SourceAbilitySystemComponent &&
			DamageParam.TargetAbilitySystemComponent)
		{
			UProjectBlueprintFunctionLibrary::ApplyDamageGameplayEffectByDamageEffectParam(DamageParam);
		}
	}

	for (const TPair<AActor*, TArray<FHitResult>>& ActorHits : HitsByActor)
	{
		AActor* HitActor = ActorHits.Key;
		if (!IsValid(HitActor))
		{
			continue;
		}

		// 场景接口只负责命中特效等表现。
		if (HitActor->GetClass()->ImplementsInterface(USceneActorInterface::StaticClass()))
		{
			ISceneActorInterface::Execute_OnHitReceived(HitActor, ActorHits.Value, AvatarActor);
		}
	}
}
