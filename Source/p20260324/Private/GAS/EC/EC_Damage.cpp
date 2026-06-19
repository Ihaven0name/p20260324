#include "GAS/EC/EC_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectGameInstance.h"
#include "GAS/ProjectAttributeSet.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/CharacterInfo.h"
#include "Interface/CharacterInterface.h"
#include "Kismet/GameplayStatics.h"

//TODO:在ANS的end进行判断是否有可以进行伤害的对象，如果有，就进行伤害
struct FProjectDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageResilience);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePenetration);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToCaptureDefMap;

	FProjectDamageStatics()
	{
		// Target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, DamageResilience, Target, false);

		// Source
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectAttributeSet, DamagePenetration, Source, false);

		const FProjectGameplayTag& GameplayTags = FProjectGameplayTag::Get();
		if (!GameplayTags.bIsInitialized)
		{
			GameplayTags.InitGameplayTags();
		}

		// ========== Target Attributes ==========
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_Defense, DefenseDef);
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_DamageResilience, DamageResilienceDef);

		// ========== Source Attributes ==========
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_AttackPower, AttackPowerDef);
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagToCaptureDefMap.Add(GameplayTags.Attribute_Secondary_DamagePenetration, DamagePenetrationDef);
	}
};

static const FProjectDamageStatics& GetProjectDamageStatics()
{
	static FProjectDamageStatics ProjectDamageStatics;
	return ProjectDamageStatics;
}
UEC_Damage::UEC_Damage()
{
	// Target
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().DamageResilienceDef);

	// Source
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetProjectDamageStatics().DamagePenetrationDef);
}


void UEC_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput
) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTag=Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag=Spec.CapturedTargetTags.GetAggregatedTags();
	const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();

	AActor* SourceAvatar=ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	AActor* TargetAvatar=ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();

	
	FGameplayEffectContextHandle Context = Spec.GetContext();
	int32 SourceLevel=1;
	int32 TargetLevel=1;
	if (SourceAvatar->Implements<UCharacterInfo>())
	{
		SourceLevel=ICharacterInterface::Execute_GetLevel(SourceAvatar);
	}
	if (TargetAvatar->Implements<UCharacterInfo>())
	{
		TargetLevel=ICharacterInterface::Execute_GetLevel(TargetAvatar);
	}
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTag;
	Params.TargetTags = TargetTag;

	float DamageMultiplier = Spec.GetSetByCallerMagnitude(ProjectGameplayTag.Caller_DamageMultiplier,true,-1);

	// ========== 捕获属性 ==========
	float SourceAttackPower = 0.f;
	float SourceCriticalHitChance = 0.f;
	float SourceCriticalHitDamage = 0.f;
	float SourceDamagePenetration = 0.f;
	float TargetDefense = 0.f;
	float TargetDamageResilience = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().AttackPowerDef, Params, SourceAttackPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().CriticalHitChanceDef, Params, SourceCriticalHitChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().CriticalHitDamageDef, Params, SourceCriticalHitDamage);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().DamagePenetrationDef, Params, SourceDamagePenetration);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().DefenseDef, Params, TargetDefense);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetProjectDamageStatics().DamageResilienceDef, Params, TargetDamageResilience);

	// ========== Clamp 防负数 ==========
	SourceAttackPower = FMath::Max(0.f, SourceAttackPower);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);
	SourceDamagePenetration = FMath::Max(0.f, SourceDamagePenetration);
	TargetDefense = FMath::Max(0.f, TargetDefense);
	TargetDamageResilience = FMath::Max(0.f, TargetDamageResilience);

	float CriticalHitMultiplier = 1.f;
	bool bIsCriticalHit = false;
	if (FMath::RandRange(0.f, 1.f) < SourceCriticalHitChance)
	{
		bIsCriticalHit=true;
		CriticalHitMultiplier+=SourceCriticalHitDamage;
	}
	float DefenseMultiplier = (SourceLevel*10+200)/(SourceLevel*10+TargetDefense+TargetLevel);
	float DamagePenetrationMultiplier = 1.f - SourceDamagePenetration - TargetDamageResilience;
	
	// ========== 最终计算（示例：防御穿透） ==========
	float Damage = SourceAttackPower * DamageMultiplier * CriticalHitMultiplier * DefenseMultiplier * DamagePenetrationMultiplier;
	UE_LOG(LogTemp,Warning,TEXT("Damage: %f"),Damage);
	FGameplayModifierEvaluatedData EvalData(UProjectAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvalData);
}