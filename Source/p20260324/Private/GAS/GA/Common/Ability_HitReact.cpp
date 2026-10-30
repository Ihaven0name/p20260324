#include "GAS/GA/Common/Ability_HitReact.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GAS/ProjectGameplayTag.h"
#include "Interface/CharacterInterface.h"
#include "Navigation/PathFollowingComponent.h"

UAbility_HitReact::UAbility_HitReact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UAbility_HitReact::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ApplyHitReactControlLock();

	const int32 DirectionValue = TriggerEventData
		? FMath::RoundToInt(TriggerEventData->EventMagnitude)
		: static_cast<int32>(EHitReactDirection::Front);
	const EHitReactDirection Direction = static_cast<EHitReactDirection>(FMath::Clamp(
		DirectionValue,
		static_cast<int32>(EHitReactDirection::Front),
		static_cast<int32>(EHitReactDirection::Right)));
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UAnimMontage* Montage = IsValid(AvatarActor) && AvatarActor->GetClass()->ImplementsInterface(UCharacterInterface::StaticClass())
		? ICharacterInterface::Execute_GetHitReactMontage(AvatarActor, Direction)
		: nullptr;

	if (!IsValid(Montage))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage);
	if (!MontageTask)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnHitReactMontageEnded);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnHitReactMontageEnded);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnHitReactMontageEnded);
	MontageTask->ReadyForActivation();
}

void UAbility_HitReact::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	RemoveHitReactControlLock();
	if (ACharacter* Character = LandingWaitCharacter.Get())
	{
		Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnCharacterLanded);
	}
	LandingWaitCharacter.Reset();
	bWaitingForLanding = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_HitReact::ApplyHitReactControlLock()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor) || !AvatarActor->HasAuthority())
	{
		return;
	}

	FGameplayTagContainer BlockedInputs;
	const FProjectGameplayTag& Tags = FProjectGameplayTag::Get();
	BlockedInputs.AddTag(Tags.Block_InputAction_Move);
	BlockedInputs.AddTag(Tags.Block_InputAction_Jump);
	BlockedInputs.AddTag(Tags.Block_InputAction_Ability);
	if (UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(AvatarActor, BlockedInputs, true))
	{
		bHitReactControlLockApplied = true;
	}

	if (APawn* Pawn = Cast<APawn>(AvatarActor))
	{
		if (AAIController* AIController = Cast<AAIController>(Pawn->GetController()))
		{
			if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
			{
				PausedBrainComponent = BrainComponent;
				BrainComponent->PauseLogic(TEXT("HitReact"));
			}
			if (UPathFollowingComponent* PathFollowing = AIController->GetPathFollowingComponent())
			{
				PathFollowing->AbortMove(
					*this,
					FPathFollowingResultFlags::ForcedScript,
					FAIRequestID::CurrentRequest,
					EPathFollowingVelocityMode::Keep);
			}
		}
	}
}

void UAbility_HitReact::RemoveHitReactControlLock()
{
	if (bHitReactControlLockApplied)
	{
		if (AActor* AvatarActor = GetAvatarActorFromActorInfo(); IsValid(AvatarActor) && AvatarActor->HasAuthority())
		{
			FGameplayTagContainer BlockedInputs;
			const FProjectGameplayTag& Tags = FProjectGameplayTag::Get();
			BlockedInputs.AddTag(Tags.Block_InputAction_Move);
			BlockedInputs.AddTag(Tags.Block_InputAction_Jump);
			BlockedInputs.AddTag(Tags.Block_InputAction_Ability);
			UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AvatarActor, BlockedInputs, true);
		}
		bHitReactControlLockApplied = false;
	}

	if (UBrainComponent* BrainComponent = PausedBrainComponent.Get())
	{
		BrainComponent->ResumeLogic(TEXT("HitReact"));
	}
	PausedBrainComponent.Reset();
}

void UAbility_HitReact::OnHitReactMontageEnded()
{
	if (bWaitingForLanding)
	{
		return;
	}

	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
			MovementComponent && MovementComponent->IsFalling())
		{
			bWaitingForLanding = true;
			LandingWaitCharacter = Character;
			Character->LandedDelegate.AddUniqueDynamic(this, &ThisClass::OnCharacterLanded);
			return;
		}
	}

	FinishHitReact();
}

void UAbility_HitReact::OnCharacterLanded(const FHitResult& Hit)
{
	FinishHitReact();
}

void UAbility_HitReact::FinishHitReact()
{
	K2_EndAbility();
}
