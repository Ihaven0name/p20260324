
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

bool UWidgetInfo::FindWidgetControllerClass(
	const FGameplayTag ControllerTag,
	TSubclassOf<UProjectBaseWidgetController>& WidgetControllerClass)
{
	if (const TSubclassOf<UProjectBaseWidgetController>* Found = ControllerTagToWidgetController.Find(ControllerTag))
	{
		WidgetControllerClass=*Found;
		return true;
	}
	return false;
}
