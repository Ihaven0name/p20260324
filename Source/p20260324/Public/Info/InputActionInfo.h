#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputActionInfo.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FActionInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ActionTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction;
};


UCLASS(BlueprintType)
class P20260324_API UInputActionInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<FActionInfo> ActionTagAndInputActions;
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	UInputAction* FindActionForTag(FGameplayTag ActionTag) const;
};
