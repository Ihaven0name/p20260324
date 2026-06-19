#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "ProjectInventoryItem.generated.h"

class UProjectBaseWidgetController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDataChanged, int32, NewQuantity);

class UInventoryItemInfo;

UCLASS(BlueprintType,Blueprintable)
class P20260324_API UProjectInventoryItem : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemDataChanged OnItemDataChanged;
	UPROPERTY()
	FGameplayTag ItemTag;
	UPROPERTY(BlueprintReadWrite)
	int32 OwnedNumber;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UProjectBaseWidgetController> WidgetController;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UInventoryItemInfo> InventoryItemInfo;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag();
	UFUNCTION(BlueprintCallable)
	UProjectBaseWidgetController* GetWidgetController() const;
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UProjectBaseWidgetController* InWidgetController);
};
