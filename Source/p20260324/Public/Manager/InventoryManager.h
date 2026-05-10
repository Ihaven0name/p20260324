#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged,const FGameplayTag, ItemTag,const int32,ItemQuantity);



USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ItemTag;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemQuantity;

	UPROPERTY(BlueprintReadWrite)
	FString ItemInstanceID; // 唯一 ID，以后服务器生成，现在可以用随机字符串
};
UCLASS()
class P20260324_API UInventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;
	
	UFUNCTION(BlueprintCallable)
	bool AddItem(const FGameplayTag ItemTag, int32 Count);
	UFUNCTION(BlueprintCallable)
	bool RemoveItem(const FGameplayTag ItemTag, int32 Count);
	UFUNCTION(BlueprintCallable)
	int32 GetItemQuantity(const FGameplayTag ItemTag) const;

private:
	// 目前是只添加进这个数组，后面的想法是在连接服务器的时候把数据同步到这个TMap上
	UPROPERTY()
	TMap<FGameplayTag, int32> ItemTagToQuantity;

	// 预留一个函数，以后在这里写网络代码
	// void SyncWithServer();
};
