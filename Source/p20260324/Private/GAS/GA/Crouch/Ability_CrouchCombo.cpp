#include "GAS/GA/Crouch/Ability_CrouchCombo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilityTask/AbilityTask_WaitReleaseAtDuration.h"
#include "Interface/PlayerInterface.h"
#include "Kismet/KismetMathLibrary.h"




void UAbility_CrouchCombo::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(),InputWindowTag,true);
	CloseToTarget();
	PlayAttackMontage();
	
}

void UAbility_CrouchCombo::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_CrouchCombo::CloseToTarget()
{
	if (bOpenChase)
	{
		if (AActor* AvatarActor=GetAvatarActorFromActorInfo())
		{
			if (AvatarActor->Implements<UPlayerInterface>())
			{
				FVector Direction;
				float Distance;
				if (IPlayerInterface::Execute_GetbIsLocking(AvatarActor))
				{
					AActor* TargetActor=IPlayerInterface::Execute_GetLockingActor(AvatarActor);
					Distance=UKismetMathLibrary::Vector_Distance(AvatarActor->GetActorLocation(),TargetActor->GetActorLocation());
					Direction = (TargetActor->GetActorLocation() - AvatarActor->GetActorLocation()).GetSafeNormal();
					Distance=FMath::Min(Distance-MinKeepDistance,LockingChaseDistance);
				}
				else
				{
					Distance=PrimaryChaseDistance;
					Direction = AvatarActor->GetActorForwardVector();
				}
				FVector TargetLocation=AvatarActor->GetActorLocation()+Distance*Direction;
				UAbilityTask_ApplyRootMotionMoveToForce* AbilityTask_ApplyRootMotionMoveToForce=UAbilityTask_ApplyRootMotionMoveToForce::ApplyRootMotionMoveToForce(
					this,
					TEXT("CrouchComboMoveTask"),
					TargetLocation,
					ChaseDuration,
					false,
					MOVE_None,
					true,
					nullptr,
					ERootMotionFinishVelocityMode::ClampVelocity,
					FVector::ZeroVector,
					0.f
				);
				AbilityTask_ApplyRootMotionMoveToForce->ReadyForActivation();
			}
		}
	}
}

void UAbility_CrouchCombo::PlayAttackMontage()
{
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(GetAvatarActorFromActorInfo(),OwnedTagInMontage,true);
	UAbilityTask_PlayMontageAndWait* AbilityTask_PlayMontageAndWait=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TEXT("CrouchComboMontageTask"),
		AbilityMontage,
		1.f,
		FName("Default")
	);
	AbilityTask_PlayMontageAndWait->OnInterrupted.AddDynamic(this,&ThisClass::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCancelled.AddDynamic(this,&ThisClass::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCompleted.AddDynamic(this,&ThisClass::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->ReadyForActivation();
}
