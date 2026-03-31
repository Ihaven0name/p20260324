#pragma once
#include "GameplayEffectTypes.h"


//TODO:后面实现
// USTRUCT(BlueprintType)
// struct FProjectGameplayEffectContext:public FGameplayEffectContext
// {
// 	GENERATED_BODY()
// public:
// 	virtual UScriptStruct* GetScriptStruct() const override
// 	{
// 		// return FGameplayEffectContext::StaticStruct();
// 		return FProjectGameplayEffectContext::StaticStruct();
// 	}
// 	/** Creates a copy of this context, used to duplicate for later modifications */
// 	virtual FProjectGameplayEffectContext* Duplicate() const override
// 	{
// 		FProjectGameplayEffectContext* NewContext = new FProjectGameplayEffectContext();
// 		*NewContext = *this;
// 		if (GetHitResult())
// 		{
// 			// Does a deep copy of the hit result
// 			NewContext->AddHitResult(*GetHitResult(), true);
// 		}
// 		return NewContext;
// 	}
// 	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
// };
// template<>
// struct TStructOpsTypeTraits<FProjectGameplayEffectContext>:public TStructOpsTypeTraitsBase2<FProjectGameplayEffectContext>
// {
// 	enum
// 	{
// 		WithNetSerializer = true,
// 		WithCopy=true
// 	};
// };