#pragma once

#include "UnLuaEx.h"
#include "GameplayTagContainer.h"


BEGIN_EXPORT_REFLECTED_CLASS(FGameplayTag)
	ADD_STATIC_FUNCTION_EX("RequestGameplayTag", FGameplayTag, RequestGameplayTag)
END_EXPORT_CLASS()

IMPLEMENT_EXPORTED_CLASS(FGameplayTag)