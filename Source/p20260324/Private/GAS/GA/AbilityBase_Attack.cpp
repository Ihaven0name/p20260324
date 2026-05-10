#include "GAS/GA/AbilityBase_Attack.h"

#include "AbilitySystemBlueprintLibrary.h"

UAbilityBase_Attack::UAbilityBase_Attack()
{
	// NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

FDamageEffectParam UAbilityBase_Attack::GetDefaultDamageEffectParam(
	AActor* Target,
	bool bOverrideKnockBackDirection,
	float KnockBackPitchOverrideMagnitude
)
{
	// 1. 从默认参数拷贝（继承编辑器中配置的所有值）
	FDamageEffectParam DamageEffectParam = DefaultDamageEffectParam;

	// 2. 覆盖动态字段
	DamageEffectParam.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParam.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParam.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	DamageEffectParam.AbilityLevel = GetAbilityLevel();

	// 3. 计算击退方向
	if (IsValid(Target))
	{
		FVector Direction = (Target->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
		if (bOverrideKnockBackDirection)
		{
			FRotator Rotation=Direction.Rotation();
			Rotation.Pitch=KnockBackPitchOverrideMagnitude;
			Direction=Rotation.Vector();
			Direction.Normalize();
		}
		DamageEffectParam.KnockBackImpulse = Direction * KnockBackMagnitude;
	}

	return DamageEffectParam;
}

