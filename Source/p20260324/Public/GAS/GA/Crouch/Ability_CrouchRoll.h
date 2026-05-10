#pragma once

#include "CoreMinimal.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "Ability_CrouchRoll.generated.h"
//Montage的方向是这个
UENUM(BlueprintType)
enum class ERollDirection : uint8
{
	Forward       = 0, // 0度
	ForwardLeft   = 1, // 45度 (逆时针)
	Left          = 2, // 90度
	BackwardLeft  = 3, // 135度
	Backward      = 4, // 180度
	BackwardRight = 5, // 225度 (-135度)
	Right         = 6, // 270度 (-90度)
	ForwardRight  = 7  // 315度 (-45度)
};

UCLASS()
class P20260324_API UAbility_CrouchRoll : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntervalTime=0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer WindowTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> RollMontages; 
	UFUNCTION(BlueprintCallable)
	void OpenInputWindowAndEndAbility();
	
};
