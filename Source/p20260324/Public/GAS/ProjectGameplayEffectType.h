#pragma once
#include "GameplayEffectTypes.h"
#include "ProjectGameplayEffectType.generated.h"


class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParam
{
    GENERATED_BODY()

	FDamageEffectParam(){};

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 AbilityLevel=1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DamageMultiplier=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DebuffChance=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DebuffDuration=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DebuffFrequency=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DebuffDamage=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float KnockBackChance=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector KnockBackImpulse=FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FProjectGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FProjectGameplayEffectContext::StaticStruct();
	}

	virtual FProjectGameplayEffectContext* Duplicate() const override
	{
		FProjectGameplayEffectContext* NewContext = new FProjectGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	bool GetIsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(const bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }

	bool GetIsApplyDebuff() const { return bIsApplyDebuff; }
	void SetIsApplyDebuff(const bool InIsApplyDebuff) { bIsApplyDebuff = InIsApplyDebuff; }

	float GetDebuffDamage() const { return DebuffDamage; }
	void SetDebuffDamage(const float InDebuffDamage) { DebuffDamage = InDebuffDamage; }

	float GetDebuffDuration() const { return DebuffDuration; }
	void SetDebuffDuration(const float InDebuffDuration) { DebuffDuration = InDebuffDuration; }

	float GetDebuffFrequency() const { return DebuffFrequency; }
	void SetDebuffFrequency(const float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }

	FVector GetKnockBackImpulse() const { return KnockBackImpulse; }
	void SetKnockBackImpulse(const FVector& InKnockBackImpulse) { KnockBackImpulse = InKnockBackImpulse; }

private:
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsApplyDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	FVector KnockBackImpulse = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FProjectGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FProjectGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
