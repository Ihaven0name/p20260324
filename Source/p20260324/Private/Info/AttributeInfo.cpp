// Fill out your copyright notice in the Description page of Project Settings.


#include "Info/AttributeInfo.h"
#include "p20260324/LogChannel.h"


bool UAttributeInfo::FindFProjectAttributeInfoByAttributeTag(
	const FGameplayTag AttributeTag,
	FProjectAttributeInfo& OutProjectAttributeInfo,
	bool bLogNotFound
)
{
	for (auto& TempAttributeInfo : AttributeInfoArray)
	{
		if (AttributeTag.MatchesTagExact(TempAttributeInfo.AttributeTag))
		{
			OutProjectAttributeInfo=TempAttributeInfo;
			return true;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogProject, Error, TEXT("Attribute For Tag[%s] not found In [%s]"),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return false;
}
