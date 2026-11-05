// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/InventoryManager.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/InventoryItemInfo.h"
#include "Manager/ConfigManager.h"
#include "p20260324/LogChannel.h"
#include "UI/Item/ProjectInventoryItem.h"





bool UInventoryManager::AddItem(const FGameplayTag ItemTag, int32 Count)
{
	//TODO:需要修改，没必要if 
	if (!ItemTag.IsValid() || Count <= 0) return false;
	if (int32* ItemAmountPtr = ItemTagToQuantity.Find(ItemTag))
	{
		const int32 TempQuantity = *ItemAmountPtr+Count;
		UInventoryItemInfo* TempInventoryItemInfo;
		if (GetInventoryInfo()->FindInventoryItemInfoByItemTag(ItemTag,TempInventoryItemInfo))
		{
			if (TempInventoryItemInfo->MaxStackNumber<TempQuantity)
			{
				*ItemAmountPtr=TempInventoryItemInfo->MaxStackNumber;
				OnInventorySingleChanged.Broadcast(ItemTag, TempInventoryItemInfo->MaxStackNumber);
				UE_LOG(LogProject,Warning,TEXT("东西超过上限，无法获得更多"))
				return false;
			}
			*ItemAmountPtr=TempQuantity;
			OnInventorySingleChanged.Broadcast(ItemTag, TempQuantity);
		}
	}
	else
	{
		ItemTagToQuantity.Add(ItemTag) += Count;
		OnInventorySingleChanged.Broadcast(ItemTag, Count);
		OnInventoryMultiChanged.Broadcast();
	}
	
	return true;
}

bool UInventoryManager::RemoveItem(const FGameplayTag ItemTag, int32 Count)
{
	if (!ItemTag.IsValid() || Count <= 0) return false;
	if (int32* ItemQuantityPtr = ItemTagToQuantity.Find(ItemTag))
	{
		if (*ItemQuantityPtr < Count)
		{
			UE_LOG(LogProject, Error, TEXT("Inventory: Attempt to remove %d of %s, but only %d exists!"), 
				Count, *ItemTag.ToString(), *ItemQuantityPtr);
			return false;
		}
		*ItemQuantityPtr -= Count;
		
		if (*ItemQuantityPtr <= 0)
		{
			ItemTagToQuantity.Remove(ItemTag);
			OnInventorySingleChanged.Broadcast(ItemTag, 0);
			OnInventoryMultiChanged.Broadcast();
		}
		else
		{
			const int32 TempQuantity = *ItemQuantityPtr;
			OnInventorySingleChanged.Broadcast(ItemTag, TempQuantity);
		}
		return true;
	}
	return false;
}

int32 UInventoryManager::GetItemQuantity(const FGameplayTag ItemTag) const
{
	if (const int32* ItemQuantityPtr = ItemTagToQuantity.Find(ItemTag))
	{
		return *ItemQuantityPtr;
	}
	UE_LOG(LogProject,Warning,TEXT("没有这个Inventory 标签"));
	return 0;
}

void UInventoryManager::GetFilterItems(const FGameplayTag ItemTypeTag, TArray<UProjectInventoryItem*>& FilterItemsInfo)
{
	if (ItemTypeTag.MatchesTagExact(FProjectGameplayTag::Get().Inventory_Empty)) return;
	FilterItemsInfo.Empty();
	for (const auto [ItemTag,ItemQuantity] : ItemTagToQuantity)
	{
		if (ItemTag.MatchesTag(ItemTypeTag))
		{
			UProjectInventoryItem* TempProjectInventoryItem=NewObject<UProjectInventoryItem>(this);
			UInventoryItemInfo* TempInventoryItemInfo=nullptr;
			if (GetInventoryInfo()->FindInventoryItemInfoByItemTag(ItemTag,TempInventoryItemInfo))
			{
				TempProjectInventoryItem->ItemTag=ItemTag;
				TempProjectInventoryItem->InventoryItemInfo=TempInventoryItemInfo;
				TempProjectInventoryItem->OwnedNumber=ItemQuantity;
				FilterItemsInfo.Add(TempProjectInventoryItem);
			}
		}
	}
}

const TMap<FGameplayTag, int32>& UInventoryManager::GetItemQuantityMap() const
{
	return ItemTagToQuantity;
}

void UInventoryManager::SyncFromAuthoritativeInventory(const TArray<FInventoryItem>& Items)
{
	TMap<FGameplayTag, int32> NewQuantities;
	for (const FInventoryItem& Item : Items)
	{
		if (Item.ItemTag.IsValid() && Item.ItemQuantity > 0)
		{
			NewQuantities.Add(Item.ItemTag, Item.ItemQuantity);
		}
	}

	TSet<FGameplayTag> ChangedTags;
	for (const TPair<FGameplayTag, int32>& Existing : ItemTagToQuantity)
	{
		if (NewQuantities.FindRef(Existing.Key) != Existing.Value)
		{
			ChangedTags.Add(Existing.Key);
		}
	}
	for (const TPair<FGameplayTag, int32>& Updated : NewQuantities)
	{
		if (ItemTagToQuantity.FindRef(Updated.Key) != Updated.Value)
		{
			ChangedTags.Add(Updated.Key);
		}
	}

	ItemTagToQuantity = MoveTemp(NewQuantities);
	for (const FGameplayTag& ChangedTag : ChangedTags)
	{
		OnInventorySingleChanged.Broadcast(ChangedTag, ItemTagToQuantity.FindRef(ChangedTag));
	}
	if (!ChangedTags.IsEmpty())
	{
		OnInventoryMultiChanged.Broadcast();
	}
}

UInventoryInfo* UInventoryManager::GetInventoryInfo()
{
	if (InventoryInfo == nullptr)
	{
		InventoryInfo=GetGameInstance()->GetSubsystem<UConfigManager>()->GetInventoryInfo();
	}
	return InventoryInfo;
}
