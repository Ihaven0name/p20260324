#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InventoryInfo.generated.h"


class UProjectCommonActivatableWidget;
class UProjectInventoryItem;
class UInventoryItemInfo;

UCLASS()
class P20260324_API UInventoryInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag,TObjectPtr<UInventoryItemInfo>> ItemTagToInventoryItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Empty")
	TObjectPtr<UInventoryItemInfo> EmptyInventoryItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Empty")
	FGameplayTag EmptyItemTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interact|Character")
	TSubclassOf<UProjectCommonActivatableWidget> InventorySelection_Character;
	UFUNCTION(BLueprintCallable)
	bool FindInventoryItemInfoByItemTag(const FGameplayTag& ItemTag,UInventoryItemInfo*& OutInventoryItemInfo);
};
