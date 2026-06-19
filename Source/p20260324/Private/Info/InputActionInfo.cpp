#include "Info/InputActionInfo.h"

UInputAction* UInputActionInfo::FindActionForTag(const FGameplayTag ActionTag) const
{
	for (const auto& [TempActionTag, TempInputAction] : ActionTagAndInputActions)
	{
		if (TempActionTag.MatchesTagExact(ActionTag)) return TempInputAction;
	}
	return nullptr;
}
