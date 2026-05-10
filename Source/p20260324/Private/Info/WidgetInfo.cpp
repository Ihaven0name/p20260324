
#include "Info/WidgetInfo.h"

bool UWidgetInfo::FindWidgetProperty(const FGameplayTag WidgetTag, FWidgetProperty& OutWidgetProperty)
{
	if (const FWidgetProperty* Found = WidgetTagToWidgetProperty.Find(WidgetTag))
	{
		OutWidgetProperty=*Found;
		return true;
	}
	return false;
}
