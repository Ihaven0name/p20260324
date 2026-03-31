#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputInfo.generated.h"


class UInputMappingContext;
enum class ETriggerType : uint8;

USTRUCT()
struct FInputData
{
	GENERATED_BODY()
	UPROPERTY()
	FGameplayTag ActionTag;
	UPROPERTY()
	FGameplayTag InputTag;
	UPROPERTY()
	FGameplayTag TriggerEventTag;
	UPROPERTY()
	bool bShift;
	UPROPERTY()
	bool bControl;
	UPROPERTY()
	bool bAlt;
};

UCLASS()
class P20260324_API UInputInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TObjectPtr<UInputMappingContext> DefaultProjectInputMappingContext;
};
