#include "GAS/ProjectAbilitySystemGlobals.h"

#include "GAS/ProjectGameplayEffectType.h"

FGameplayEffectContext* UProjectAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FProjectGameplayEffectContext();
	
}
