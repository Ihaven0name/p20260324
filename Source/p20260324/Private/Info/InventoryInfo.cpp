#include "Info/InventoryInfo.h"
#include "Info/InventoryItemInfo.h"
#include "GameplayTagContainer.h"

bool UInventoryInfo::FindInventoryItemInfoByItemTag(const FGameplayTag& ItemTag, UInventoryItemInfo*& OutInventoryItemInfo)
{
	if (!ItemTag.IsValid()) return false;
	auto* FoundPtr = ItemTagToInventoryItemInfo.Find(ItemTag);
	if (FoundPtr != nullptr && FoundPtr->Get() != nullptr)
	{
		OutInventoryItemInfo = FoundPtr->Get();
		return true;
	}
	OutInventoryItemInfo = nullptr;
	return false;
}