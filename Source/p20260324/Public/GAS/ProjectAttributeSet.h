#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "ProjectAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


// ==========================================
// FGameplayEffectInfo - GE 信息结构体
// ==========================================
USTRUCT(BlueprintType)
struct FGameplayEffectInfo
{
	GENERATED_BODY()

	// Source
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACharacter> SourceCharacter;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AController> SourcePlayerController;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	// Target
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<ACharacter> TargetCharacter;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AController> TargetPlayerController;
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly)
	FGameplayEffectContextHandle GameplayEffectContextHandle;
};

UCLASS()
class P20260324_API UProjectAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UProjectAttributeSet();

	// ==========================================
	// 复制相关
	// ==========================================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ==========================================
	// 属性变化回调
	// ==========================================
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ==========================================
	// Primary Attributes
	// ==========================================

	// 等级
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Level, Category="Attribute")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, Level);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetLevelValueForLua() const 
	{ 
		return GetLevel(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetLevelValueForLua(const float InValue) 
	{ 
		SetLevel(InValue); 
	}

	// 经验值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_XP, Category="Attribute")
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, XP);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetXPValueForLua() const 
	{ 
		return GetXP(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetXPValueForLua(const float InValue) 
	{ 
		SetXP(InValue); 
	}

	// 生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, Health);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetHealthValueForLua() const 
	{ 
		return GetHealth(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetHealthValueForLua(const float InValue) 
	{ 
		SetHealth(InValue); 
	}

	// 魔法值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, Mana);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetManaValueForLua() const 
	{ 
		return GetMana(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetManaValueForLua(const float InValue) 
	{ 
		SetMana(InValue); 
	}

	// 最大生命值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, MaxHealth);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetMaxHealthValueForLua() const 
	{ 
		return GetMaxHealth(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetMaxHealthValueForLua(const float InValue) 
	{ 
		SetMaxHealth(InValue); 
	}

	// 最大魔法值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, MaxMana);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetMaxManaValueForLua() const 
	{ 
		return GetMaxMana(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetMaxManaValueForLua(const float InValue) 
	{ 
		SetMaxMana(InValue); 
	}

	// ==========================================
	// Secondary Attributes
	// ==========================================

	// 攻击力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackPower, Category="Attribute")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, AttackPower);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetAttackPowerValueForLua() const 
	{ 
		return GetAttackPower(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetAttackPowerValueForLua(const float InValue) 
	{ 
		SetAttackPower(InValue); 
	}

	// 防御力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, Category="Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, Defense);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetDefenseValueForLua() const 
	{ 
		return GetDefense(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetDefenseValueForLua(const float InValue) 
	{ 
		SetDefense(InValue); 
	}

	// 伤害加成
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamageBonus, Category="Attribute")
	FGameplayAttributeData DamageBonus;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, DamageBonus);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetDamageBonusValueForLua() const 
	{ 
		return GetDamageBonus(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetDamageBonusValueForLua(const float InValue) 
	{ 
		SetDamageBonus(InValue); 
	}

	// 伤害抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamageResilience, Category="Attribute")
	FGameplayAttributeData DamageResilience;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, DamageResilience);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetDamageResilienceValueForLua() const 
	{ 
		return GetDamageResilience(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetDamageResilienceValueForLua(const float InValue) 
	{ 
		SetDamageResilience(InValue); 
	}

	// 暴击率
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Attribute")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, CriticalHitChance);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetCriticalHitChanceValueForLua() const 
	{ 
		return GetCriticalHitChance(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetCriticalHitChanceValueForLua(const float InValue) 
	{ 
		SetCriticalHitChance(InValue); 
	}

	// 暴击伤害
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Attribute")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, CriticalHitDamage);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetCriticalHitDamageValueForLua() const 
	{ 
		return GetCriticalHitDamage(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetCriticalHitDamageValueForLua(const float InValue) 
	{ 
		SetCriticalHitDamage(InValue); 
	}

	// 伤害穿透
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DamagePenetration, Category="Attribute")
	FGameplayAttributeData DamagePenetration;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, DamagePenetration);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetDamagePenetrationValueForLua() const 
	{ 
		return GetDamagePenetration(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetDamagePenetrationValueForLua(const float InValue) 
	{ 
		SetDamagePenetration(InValue); 
	}

	// ==========================================
	// Meta Attributes (不复制，用于临时计算)
	// ==========================================

	// 传入伤害
	UPROPERTY(BlueprintReadOnly, Category="Attribute|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, IncomingDamage);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetIncomingDamageValueForLua() const 
	{ 
		return GetIncomingDamage(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetIncomingDamageValueForLua(const float InValue) 
	{ 
		SetIncomingDamage(InValue); 
	}

	// 传入经验值
	UPROPERTY(BlueprintReadOnly, Category="Attribute|Meta")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UProjectAttributeSet, IncomingXP);

	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	float GetIncomingXPValueForLua() const 
	{ 
		return GetIncomingXP(); 
	} 
	UFUNCTION(BlueprintCallable, Category="Attribute|Lua") 
	void SetIncomingXPValueForLua(const float InValue) 
	{ 
		SetIncomingXP(InValue); 
	}

public:
	// ==========================================
	// Effect Info - 当前生效的 GE 信息
	// ==========================================
	UPROPERTY(BlueprintReadOnly, Category="Attribute|GEInfo")
	FGameplayEffectInfo EffectInfo;

	// 设置 GE 信息
	void SetGameplayEffectInfoFromData(const FGameplayEffectModCallbackData& Data);

protected:
	// ==========================================
	// OnRep 函数
	// ==========================================
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_DamageBonus(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_DamageResilience(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_DamagePenetration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_XP(const FGameplayAttributeData& OldValue) const;

};
