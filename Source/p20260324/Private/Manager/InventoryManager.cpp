// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/InventoryManager.h"

#include "p20260324/LogChannel.h"

bool UInventoryManager::AddItem(const FGameplayTag ItemTag, int32 Count)
{
	//TODO:需要修改，没必要if 
	if (!ItemTag.IsValid() || Count <= 0) return false;
	int32 TempQuantity;
	if (int32* ItemAmountPtr = ItemTagToQuantity.Find(ItemTag))
	{
		*ItemAmountPtr += Count;
		TempQuantity=*ItemAmountPtr;
	}
	else
	{
		ItemTagToQuantity.FindOrAdd(ItemTag) += Count;
		TempQuantity=Count;
	}
	OnInventoryChanged.Broadcast(ItemTag, TempQuantity);
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
		const int32 TempQuantity = *ItemQuantityPtr;
		if (*ItemQuantityPtr <= 0)
		{
			ItemTagToQuantity.Remove(ItemTag);
		}
		OnInventoryChanged.Broadcast(ItemTag, TempQuantity);
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
