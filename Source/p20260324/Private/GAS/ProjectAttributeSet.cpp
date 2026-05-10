#include "GAS/ProjectAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"

UProjectAttributeSet::UProjectAttributeSet()
{
}

void UProjectAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, DamageBonus, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, DamageResilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UProjectAttributeSet, DamagePenetration, COND_None, REPNOTIFY_Always);
}

void UProjectAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制 Health 范围
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	// 限制 Mana 范围
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	// 限制暴击率为 0-100
	else if (Attribute == GetCriticalHitChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
	}
	// 限制等级 >= 1
	else if (Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	// 限制经验值 >= 0
	else if (Attribute == GetXPAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UProjectAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UProjectAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 更新 GE Iiunfo
	SetGameplayEffectInfoFromData(Data);

	// 处理 IncomingDamage
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float TempIncomingDamage = Data.EvaluatedData.Magnitude;
		if (TempIncomingDamage > 0.f)
		{
			// 扣除 Health，Clamp 下限为 0
			float NewHealth = FMath::Clamp(GetHealth() - TempIncomingDamage, 0.f, GetMaxHealth());
			SetHealth(NewHealth);
			FVector KnockBackImpulse=UProjectBlueprintFunctionLibrary::GetKnockBackImpulse(EffectInfo.GameplayEffectContextHandle);
			if (KnockBackImpulse.Length()>0.1f)
			{
				EffectInfo.TargetCharacter->GetCharacterMovement()->Launch(KnockBackImpulse);
			}
		}
		// 清零 IncomingDamage（Meta Attribute 是临时的）
		SetIncomingDamage(0.f);
	}
	// 处理 IncomingXP
	else if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const int32 TempXP=Data.EffectSpec.GetSetByCallerMagnitude(FProjectGameplayTag::Get().Attribute_Meta_IncomingXP);
		int32 CurrentLevel;
		int32 CurrentXP;
		UProjectBlueprintFunctionLibrary::CalculateCurrentLevel(this,TempXP,CurrentLevel,CurrentXP);
		SetLevel(CurrentLevel);
		SetXP(CurrentXP);
		SetIncomingXP(0.f);
	}
}

void UProjectAttributeSet::SetGameplayEffectInfoFromData(const FGameplayEffectModCallbackData& Data)
{
	EffectInfo.GameplayEffectContextHandle = Data.EffectSpec.GetContext();
	if (!EffectInfo.GameplayEffectContextHandle.IsValid()) return;

	// Source
	EffectInfo.SourceAbilitySystemComponent = EffectInfo.GameplayEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (EffectInfo.SourceAbilitySystemComponent.IsValid() &&
		EffectInfo.SourceAbilitySystemComponent->AbilityActorInfo.IsValid() &&
		EffectInfo.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectInfo.SourcePlayerController = EffectInfo.SourceAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		EffectInfo.SourceCharacter = Cast<ACharacter>(EffectInfo.SourceAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get());
	}


	EffectInfo.TargetAbilitySystemComponent=&Data.Target;
	// Target
	if (EffectInfo.TargetAbilitySystemComponent.IsValid() &&
		EffectInfo.TargetAbilitySystemComponent->AbilityActorInfo.IsValid() &&
		EffectInfo.TargetAbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectInfo.TargetPlayerController =EffectInfo.TargetAbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		EffectInfo.TargetCharacter = Cast<ACharacter>(EffectInfo.TargetAbilitySystemComponent->AbilityActorInfo->AvatarActor.Get());
	}
}

// ==========================================
// OnRep 函数实现
// ==========================================
void UProjectAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, Health, OldValue);
}

void UProjectAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, AttackPower, OldValue);
}

void UProjectAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, Mana, OldValue);
}

void UProjectAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, Defense, OldValue);
}

void UProjectAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, MaxHealth, OldValue);
}

void UProjectAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, MaxMana, OldValue);
}

void UProjectAttributeSet::OnRep_DamageBonus(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, DamageBonus, OldValue);
}

void UProjectAttributeSet::OnRep_DamageResilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, DamageResilience, OldValue);
}

void UProjectAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, CriticalHitChance, OldValue);
}

void UProjectAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, CriticalHitDamage, OldValue);
}

void UProjectAttributeSet::OnRep_DamagePenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, DamagePenetration, OldValue);
}

void UProjectAttributeSet::OnRep_Level(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, Level, OldValue);
}

void UProjectAttributeSet::OnRep_XP(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UProjectAttributeSet, XP, OldValue);
}
