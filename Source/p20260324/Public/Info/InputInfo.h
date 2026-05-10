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

USTRUCT(BlueprintType)
struct FInputLoadSaveData
{
	GENERATED_BODY()
	UPROPERTY()
	FGameplayTag ActionTag;
	UPROPERTY()
	FGameplayTag InputTag;
	UPROPERTY()
	bool bShift;
	UPROPERTY()
	bool bControl;
	UPROPERTY()
	bool bAlt;
};

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
struct FActionInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag ActionTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInputAction> InputAction;
};



USTRUCT(BlueprintType)
struct FInputSetting
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FInputActionAndAbilityInputData> InputActionAndAbilityInputDataArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInputMappingContext>SpecialProjectInputMappingContext;
};

UCLASS()
class P20260324_API UInputInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TObjectPtr<UInputMappingContext> DefaultProjectAttackInputMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TObjectPtr<UInputMappingContext> DefaultProjectUIInputMappingContext;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="InputMappingContext")
	TMap<EPlayerCategory,FInputSetting> PlayerCategoryToInputSetting;
	
};
