#pragma once

#include "CoreMinimal.h"
#include "CommonInputModeTypes.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "InputInfo.generated.h"


enum class EPlayerCategory;
class UInputAction;
class UInputMappingContext;

enum class ETriggerType : uint8;


//TODO:后面需要添加所有的需要的Tag，包括自己创建的ActivationHasAnyTags，在GA的构造函数读取这些东西，然后进行初始化，这样可以方便配置
USTRUCT(BlueprintType)
struct FAbilityInputData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass = nullptr;
};

USTRUCT(BlueprintType)
struct FInputActionAndAbilityInputData
{
	GENERATED_BODY()
	//一个InputAction对应很多个AbilityTag，比如Combo GAs
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> JudgeAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag JudgeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FAbilityInputData> AbilityInputData;
};


USTRUCT(BlueprintType)
struct FInputSetting
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FInputActionAndAbilityInputData> InputActionAndAbilityInputDataArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag SpecialIMCTag;
	
};

USTRUCT(BlueprintType)
struct FInputMappingContextWithPriority
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputMappingContext")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InputMappingContext")
	int32 InputMappingPriority = 0;
};

UCLASS()
class P20260324_API UInputInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TObjectPtr<UInputMappingContext> DefaultProjectAttackInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TriggerAction")
	TObjectPtr<UInputAction> AltTriggerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TriggerAction")
	TObjectPtr<UInputAction> ControlTriggerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TriggerAction")
	TObjectPtr<UInputAction> ShiftTriggerAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TMap<FGameplayTag,FInputMappingContextWithPriority> IMCTagToMappableInputMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TMap<FGameplayTag,FInputMappingContextWithPriority> IMCTagToNonMappableInputMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TMap<EPlayerCategory,FInputSetting> PlayerCategoryToInputSetting;

	UFUNCTION(BlueprintPure, Category="InputMappingContext")
	bool GetMappableIMC(const FGameplayTag& IMCTag, UInputMappingContext*& OutIMC) const;
	UFUNCTION(BlueprintPure, Category="InputMappingContext")
	bool GetIMCWithPriority(const FGameplayTag& IMCTag, UInputMappingContext*& OutIMC, int32& OutPriority) const;
	
};
