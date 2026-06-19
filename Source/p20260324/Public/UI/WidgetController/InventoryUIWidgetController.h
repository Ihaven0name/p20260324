#pragma once

#include "CoreMinimal.h"
#include "MultiControlWidgetController.h"
#include "SingleControlWidgetController.h"
#include "Player/ProjectPlayerState.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "InventoryUIWidgetController.generated.h"


class UProjectInventoryItem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemTypeChangedSignature, const FGameplayTag, ItemTypeTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsumableItemUsedSignature,const FTeamCharacterInfo&, TeamCharacterInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpecCharacterAttributeChangeSignature,AProjectPlayerCharacter*,PlayerCharacter,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConsumableItemNumberChangedSignature, const FGameplayTag, ItemTag, int32, ChangedNumber);

UCLASS()
class P20260324_API UInventoryUIWidgetController : public UMultiControlWidgetController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnItemTypeChangedSignature OnItemTypeChangedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnConsumableItemUsedSignature OnConsumableItemUsedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnConsumableItemNumberChangedSignature OnConsumableItemNumberChangedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSpecCharacterAttributeChangeSignature OnSpecCharacterHealthChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSpecCharacterAttributeChangeSignature OnSpecCharacterManaChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSpecCharacterAttributeChangeSignature OnSpecCharacterMaxHealthChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSpecCharacterAttributeChangeSignature OnSpecCharacterMaxManaChangeSignature;
	
	virtual void BindCallback() override;
	UFUNCTION(BlueprintCallable)
	void ApplyConsumableToTeamCharacter(UProjectInventoryItem* SelectedItem,FTeamCharacterInfo& TeamCharacterInfo);

	UFUNCTION(BlueprintCallable)
	void BroadcastCharacterHealthInitialValue();
	UFUNCTION(BlueprintCallable)
	void BroadcastCharacterManaInitialValue();
	UFUNCTION(BlueprintCallable)
	void BroadcastCharacterMaxHealthInitialValue();
	UFUNCTION(BlueprintCallable)
	void BroadcastCharacterMaxManaInitialValue();
	UFUNCTION(BlueprintCallable)
	void BroadcastSpecCharacterInitialValue(AProjectPlayerCharacter* PlayerCharacter);
	UFUNCTION()
	void OnConsumableItemNumberChangedFunction(const FGameplayTag ItemTag, int32 ChangedNumber);


};
