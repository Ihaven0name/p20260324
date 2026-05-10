#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ProjectAbilitySystemGlobals.generated.h"

UCLASS()
class P20260324_API UProjectAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
