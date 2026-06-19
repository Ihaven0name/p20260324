#pragma once

#include "UnLuaEx.h"
#include "GameplayTagContainer.h"


BEGIN_EXPORT_REFLECTED_CLASS(FGameplayTag)
	ADD_FUNCTION_EX("MatchesTag",bool,MatchesTag,FGameplayTag)
	ADD_FUNCTION_EX("MatchesTagExact",bool,MatchesTagExact,FGameplayTag)
	ADD_FUNCTION_EX("GetSingleTagContainer",FGameplayTagContainer,GetSingleTagContainer)
	ADD_FUNCTION_EX("GetTagName",FName,GetTagName)
	ADD_STATIC_FUNCTION_EX("RequestGameplayTag", FGameplayTag, RequestGameplayTag)
END_EXPORT_CLASS()

IMPLEMENT_EXPORTED_CLASS(FGameplayTag)