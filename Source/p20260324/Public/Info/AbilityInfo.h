#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

/**
 * Ability 标签配置（DataTable 行结构体）
 */
USTRUCT(BlueprintType)
struct FAbilityTagConfig : public FTableRowBase
{
	GENERATED_BODY()

	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationHasAnyTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer InputWindowTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer OwnedTagInMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer AbilityTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer CancelAbilitiesWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer BlockAbilitiesWithTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationOwnedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationRequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer ActivationBlockedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer SourceRequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer SourceBlockedTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer TargetRequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer TargetBlockedTags;
};

USTRUCT(BlueprintType)
struct FProjectAbilityInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag=FGameplayTag();
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag=FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CoolDownTag=FGameplayTag();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon=nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) 
	TObjectPtr<const UMaterialInstance> BackgroundMaterialInstance=nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass=nullptr;

	//TODO:在这里存储需要的初始化GA的需要的Tag的东西，在进行加载的时候进行初始化
};


UCLASS()
class P20260324_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FProjectAbilityInfo> AbilityOffensiveInformation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FProjectAbilityInfo> AbilityPassiveInformation;
	
	UFUNCTION(BlueprintCallable)
	bool FindAbilityInfoByAbilityTag(const FGameplayTag AbilityTag,FProjectAbilityInfo& OutProjectAbilityInfo,bool bLogNotFound=false);
};
