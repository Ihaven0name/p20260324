


#include "GAS/GA/Crouch/Ability_CrouchDash.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ProjectBaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ProjectGameplayTag.h"

UAbility_CrouchDash::UAbility_CrouchDash()
{
	
}

void UAbility_CrouchDash::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	const bool bCommitSucceeded = CommitAbility(Handle, ActorInfo, ActivationInfo);
	if (!bCommitSucceeded)
	{
		K2_EndAbility();
		return;
	}
	if (AActor* AvatarActorForWindow = GetAvatarActorFromActorInfo())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			FGameplayTagContainer HeldWindowTags;
			for (const FGameplayTag& WindowTag : InputWindowTag)
			{
				if (ASC->GetTagCount(WindowTag) > 0)
				{
					HeldWindowTags.AddTag(WindowTag);
				}
			}
			if (!HeldWindowTags.IsEmpty())
			{
				UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(AvatarActorForWindow, HeldWindowTags, AvatarActorForWindow->HasAuthority());
			}
		}
	}
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(
		AvatarActor,
		OwnedTagInMontage,
		AvatarActor && AvatarActor->HasAuthority());
	UAbilityTask_PlayMontageAndWait* AbilityTask_PlayMontageAndWait=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AbilityMontage,
		1.f,
		FName("Default")
	);
	AbilityTask_PlayMontageAndWait->OnInterrupted.AddDynamic(this,&UAbility_CrouchDash::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCancelled.AddDynamic(this,&UAbility_CrouchDash::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCompleted.AddDynamic(this,&UAbility_CrouchDash::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* AbilityTask_WaitGameplayEvent=UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		FProjectGameplayTag::Get().Event_Crouch_Dash_AddLaunch
	);
	AbilityTask_WaitGameplayEvent->EventReceived.AddDynamic(this,&UAbility_CrouchDash::AddLaunch);
	AbilityTask_WaitGameplayEvent->ReadyForActivation();
}

void UAbility_CrouchDash::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_CrouchDash::AddLaunch(FGameplayEventData)
{
	AProjectBaseCharacter* Avatar = CastChecked<AProjectBaseCharacter>(GetAvatarActorFromActorInfo());
	FRotator LaunchRotation=Avatar->GetActorForwardVector().Rotation();
	LaunchRotation.Pitch=DashPitchOverride;
	const FVector LaunchDirection=LaunchRotation.Vector();
	UCharacterMovementComponent* Movement = Avatar->GetCharacterMovement();
	Movement->Launch(LaunchDirection * DashForce);
}
