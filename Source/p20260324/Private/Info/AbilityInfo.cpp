#include "Info/AbilityInfo.h"

#include "p20260324/LogChannel.h"


bool UAbilityInfo::FindAbilityInfoByAbilityTag(
	const FGameplayTag AbilityTag,
	FProjectAbilityInfo& OutProjectAbilityInfo,
	bool bLogNotFound
)
{
	for (const FProjectAbilityInfo& TempAuroAbilityInfo : AbilityOffensiveInformation)
	{
		if (TempAuroAbilityInfo.AbilityTag==AbilityTag)
		{
			OutProjectAbilityInfo = TempAuroAbilityInfo;
			return true;
		}
	}
	for (const FProjectAbilityInfo& TempAuroAbilityInfo : AbilityPassiveInformation)
	{
		if (TempAuroAbilityInfo.AbilityTag==AbilityTag)
		{
			OutProjectAbilityInfo = TempAuroAbilityInfo;
			return true;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogProject, Error, TEXT("Attribute For Tag[%s] not found In [%s]"),*AbilityTag.ToString(),*GetNameSafe(this));
	}
	return false;
}
