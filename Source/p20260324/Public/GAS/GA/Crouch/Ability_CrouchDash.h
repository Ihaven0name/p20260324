// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/AbilityBase_Attack.h"
#include "Ability_CrouchDash.generated.h"

/**
 * 
 */
UCLASS()
class P20260324_API UAbility_CrouchDash : public UAbilityBase_Attack
{
	GENERATED_BODY()
public:
	UAbility_CrouchDash();
	
	UPROPERTY(EditAnywhere)
	float DashForce=1000.f;
	UPROPERTY(EditAnywhere)
	float DashPitchOverride=10.f;

	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;


	UFUNCTION(BlueprintCallable)
	void AddLaunch(FGameplayEventData Payload);
};
