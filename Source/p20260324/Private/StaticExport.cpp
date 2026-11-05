#pragma once

#include "UnLuaEx.h"
#include "GameplayTagContainer.h"

namespace
{
	FGameplayTagContainer GetSingleTagContainerCopy(const FGameplayTag& Tag)
	{
		return Tag.GetSingleTagContainer();
	}
}

BEGIN_EXPORT_REFLECTED_CLASS(FGameplayTag)
	ADD_CONST_FUNCTION_EX("MatchesTag",bool,MatchesTag,const FGameplayTag&)
	ADD_CONST_FUNCTION_EX("MatchesTagExact",bool,MatchesTagExact,const FGameplayTag&)
	ADD_EXTERNAL_FUNCTION_EX("GetSingleTagContainer",FGameplayTagContainer,GetSingleTagContainerCopy,const FGameplayTag&)
	ADD_CONST_FUNCTION_EX("GetTagName",FName,GetTagName)
	ADD_STATIC_FUNCTION_EX("RequestGameplayTag", FGameplayTag, RequestGameplayTag)
END_EXPORT_CLASS()

IMPLEMENT_EXPORTED_CLASS(FGameplayTag)
