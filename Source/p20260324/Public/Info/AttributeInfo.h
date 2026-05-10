
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FProjectAttributeInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag=FGameplayTag();
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue=0.0f;
};

UCLASS(Blueprintable)
class P20260324_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FProjectAttributeInfo> AttributeInfoArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> AddIncomingXPEffectClass;
	UFUNCTION(BlueprintCallable)
	bool FindFProjectAttributeInfoByAttributeTag(const FGameplayTag AttributeTag,FProjectAttributeInfo& OutProjectAttributeInfo,bool bLogNotFound=false);
	
};
