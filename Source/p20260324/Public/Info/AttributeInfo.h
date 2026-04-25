
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FProjectAttributeInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag=FGameplayTag();
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue=0.0f;
};

UCLASS()
class P20260324_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
};
