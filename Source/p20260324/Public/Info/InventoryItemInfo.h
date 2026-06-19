#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GAS/ProjectGameplayTag.h"
#include "InventoryItemInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;


//单个东西的静态资源
UCLASS()
class P20260324_API UInventoryItemInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Identity")
	FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Identity")
	FGameplayTag ItemTypeTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Identity")
	int32 MaxStackNumber=99999;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Resource")
	TSoftObjectPtr<UTexture2D> IconTexture;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Interact")
	FText DescriptionText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Interact")
	FText ItemName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Interact")
	bool bIsConsumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", 
		meta = (EditCondition = "bIsConsumable", EditConditionHides))
	TSubclassOf<UGameplayEffect> ConsumableEffect = nullptr;

	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UInventoryItemInfo, ItemTag))
		{
			UpdateEditorStates();
		}
	}
	virtual void PostLoad() override
	{
		Super::PostLoad();
		UpdateEditorStates();
	}
	void UpdateEditorStates()
	{
		bIsConsumable = ItemTag.MatchesTag(FProjectGameplayTag::Get().Inventory_Consumable);
	}
#endif
	
};
