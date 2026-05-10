#include "GAS/GA/Crouch/Ability_CrouchRoll.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ProjectThreadSafeFunctionLibrary.h"
#include "Interface/CharacterInterface.h"



void UAbility_CrouchRoll::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
)
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	
	float Angle=UProjectThreadSafeFunctionLibrary::GetAngleFromVectors(Character->GetActorForwardVector(),Character->GetCharacterMovement()->GetCurrentAcceleration());
	if (Angle<0)
	{
		Angle+=360.f;
	}
	Angle+=22.5f;
	
	Angle=FMath::Fmod(Angle,360.f);
	Angle/=45.f;
	const uint8 RollDirection=static_cast<uint8>(Angle);
	
	UAbilityTask_PlayMontageAndWait* AbilityTask_PlayMontageAndWait=UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		RollMontages[RollDirection],
		1.f, 
		FName("Default")
	);
	AbilityTask_PlayMontageAndWait->OnInterrupted.AddDynamic(this,&UAbility_CrouchRoll::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCancelled.AddDynamic(this,&UAbility_CrouchRoll::RemoveMontageTagAndEndAbility);
	AbilityTask_PlayMontageAndWait->OnCompleted.AddDynamic(this,&UAbility_CrouchRoll::OpenInputWindowAndEndAbility);
	AbilityTask_PlayMontageAndWait->ReadyForActivation();
}

void UAbility_CrouchRoll::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled
)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbility_CrouchRoll::OpenInputWindowAndEndAbility()
{
	if (GetAvatarActorFromActorInfo()->Implements<UCharacterInterface>())
	{
		ICharacterInterface::Execute_AddInputWindowTimer(GetAvatarActorFromActorInfo(),IntervalTime,WindowTags);
	}
	RemoveMontageTagAndEndAbility();
}
