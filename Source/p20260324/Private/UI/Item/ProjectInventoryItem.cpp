#include "UI/Item/ProjectInventoryItem.h"

#include "Info/InventoryItemInfo.h"

FGameplayTag UProjectInventoryItem::GetItemTag()
{
	if (ItemTag.IsValid()) return ItemTag;
	ItemTag = InventoryItemInfo.Get()->ItemTag;
	return ItemTag;
}

UProjectBaseWidgetController* UProjectInventoryItem::GetWidgetController() const
{
	return WidgetController;
}

void UProjectInventoryItem::SetWidgetController(UProjectBaseWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
}
