


#include "GAS/GA/Crouch/Ability_CrouchDash.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectBaseCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
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
	const APawn* AvatarPawn = Cast<APawn>(AvatarActor);
	const bool bCommitSucceeded = CommitAbility(Handle, ActorInfo, ActivationInfo);
	UE_LOG(LogTemp, Warning,
		TEXT("[DashDiag] Activate Ability=%s Actor=%s NetMode=%d LocalRole=%d LocalControlled=%d Commit=%d ActivationMode=%d"),
		*GetNameSafe(this), *GetNameSafe(AvatarActor),
		AvatarActor ? static_cast<int32>(AvatarActor->GetNetMode()) : -1,
		AvatarActor ? static_cast<int32>(AvatarActor->GetLocalRole()) : -1,
	AvatarPawn ? AvatarPawn->IsLocallyControlled() : 0,
		bCommitSucceeded,
		static_cast<int32>(ActivationInfo.ActivationMode));
	if (!bCommitSucceeded) K2_EndAbility();
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),InputWindowTag,true);
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(),OwnedTagInMontage,true);
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

void UAbility_CrouchDash::AddLaunch(FGameplayEventData Payload)
{
	AProjectBaseCharacter* Avatar = CastChecked<AProjectBaseCharacter>(GetAvatarActorFromActorInfo());
	FRotator LaunchRotation=Avatar->GetActorForwardVector().Rotation();
	LaunchRotation.Pitch=DashPitchOverride;
	const FVector LaunchDirection=LaunchRotation.Vector();
	UCharacterMovementComponent* Movement = Avatar->GetCharacterMovement();
	UE_LOG(LogTemp, Warning,
		TEXT("[DashDiag] AddLaunch Actor=%s NetMode=%d LocalRole=%d LocalControlled=%d EventTag=%s ActorRotation=%s Forward=%s Direction=%s SpeedBefore=%s Accel=%s MovementMode=%d DashForce=%.1f"),
		*GetNameSafe(Avatar), static_cast<int32>(Avatar->GetNetMode()),
		static_cast<int32>(Avatar->GetLocalRole()), Avatar->IsLocallyControlled(),
		*Payload.EventTag.ToString(), *Avatar->GetActorRotation().ToCompactString(),
		*Avatar->GetActorForwardVector().ToCompactString(), *LaunchDirection.ToCompactString(),
		*Movement->Velocity.ToCompactString(), *Movement->GetCurrentAcceleration().ToCompactString(),
		static_cast<int32>(Movement->MovementMode), DashForce);
	Movement->Launch(LaunchDirection * DashForce);
	UE_LOG(LogTemp, Warning, TEXT("[DashDiag] LaunchReturned Actor=%s VelocityNow=%s MovementModeNow=%d"),
		*GetNameSafe(Avatar), *Movement->Velocity.ToCompactString(),
		static_cast<int32>(Movement->MovementMode));
}
