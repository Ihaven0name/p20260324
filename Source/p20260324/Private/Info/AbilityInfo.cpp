#include "Info/AbilityInfo.h"

#include "Editor.h"
#include "Manager/AbilityTagManager.h"
#include "p20260324/LogChannel.h"

//FAbilityTagConfig
void FAbilityTagConfig::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
	FTableRowBase::OnDataTableChanged(InDataTable, InRowName);

#if WITH_EDITOR
	if (InRowName.IsNone() || !GEditor)
	{
		return;
	}

	if (UAbilityTagManager* AbilityTagManager = GEditor->GetEditorSubsystem<UAbilityTagManager>())
	{
		AbilityTagManager->RefreshSingleAbilityTagInCDO(InRowName);
	}
#endif
}

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
