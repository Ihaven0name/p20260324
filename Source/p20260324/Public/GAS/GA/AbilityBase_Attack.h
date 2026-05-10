#pragma once

#include "CoreMinimal.h"
#include "GAS/ProjectGameplayEffectType.h"
#include "GAS/GA/GameplayAbilityBase.h"
#include "AbilityBase_Attack.generated.h"

UCLASS()
class P20260324_API UAbilityBase_Attack : public UGameplayAbilityBase
{
	GENERATED_BODY()
public:
	UAbilityBase_Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageEffectParam DefaultDamageEffectParam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideKnockBack=false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockBackMagnitude=100.f;
	UPROPERTY(EditAnywhere)
	float KnockBackPitchOverride=10.f;

	UFUNCTION(BlueprintCallable)
	FDamageEffectParam GetDefaultDamageEffectParam(
		AActor* Target,
		bool bOverrideKnockBackDirection = false,
		float KnockBackPitchOverrideMagnitude=0.f
	);

};
