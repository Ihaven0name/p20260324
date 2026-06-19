#include "Info/InputInfo.h"

bool UInputInfo::GetMappableIMC(const FGameplayTag& IMCTag, UInputMappingContext*& OutIMC) const
{
	int32 TempPriority = 0;
	return GetIMCWithPriority(IMCTag, OutIMC, TempPriority);
}

bool UInputInfo::GetIMCWithPriority(const FGameplayTag& IMCTag, UInputMappingContext*& OutIMC, int32& OutPriority) const
{
	OutIMC = nullptr;
	OutPriority = 0;

	if (!IMCTag.IsValid()) return false;

	if (const FInputMappingContextWithPriority* FoundMappableIMC = IMCTagToMappableInputMappingContext.Find(IMCTag))
	{
		OutIMC = FoundMappableIMC->InputMappingContext;
		OutPriority = FoundMappableIMC->InputMappingPriority;
		return OutIMC != nullptr;
	}

	if (const FInputMappingContextWithPriority* FoundNonMappableIMC = IMCTagToNonMappableInputMappingContext.Find(IMCTag))
	{
		OutIMC = FoundNonMappableIMC->InputMappingContext;
		OutPriority = FoundNonMappableIMC->InputMappingPriority;
		return OutIMC != nullptr;
	}

	return false;
}
