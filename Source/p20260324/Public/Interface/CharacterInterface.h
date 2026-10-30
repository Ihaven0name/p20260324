#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

class UProjectAbilitySystemComponent;
class UAnimMontage;
struct FGameplayTagContainer;

UENUM(BlueprintType)
enum class EHitReactDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};

UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class P20260324_API ICharacterInterface
{
	GENERATED_BODY()
public:

	// ============ 等级 & 经验 ========================
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	int32 GetLevel();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	void AddLevel(const int32 InLevelAmount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	void SetLevel(const int32 InLevelNumber);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	int32 GetXP();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	void AddXP(const int32 InXPAmount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Level")
	void SetXP(const int32 InXPNumber);
	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim")
	UProjectAbilitySystemComponent* GetAbilitySystemComponentByInterface();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim|Hit React")
	UAnimMontage* GetHitReactMontage(EHitReactDirection Direction);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim|Death")
	UAnimMontage* GetDeathMontage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Death")
	bool IsDead() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character|Death")
	void SetIsDead();
	//添加InputWindowTimer,如果已经有了Timer，先进行清除，并且进行清理InputWindow对应的标签，给BaseCharacter里面的InputWindowTimerHandle进行赋值
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim")
	void AddInputWindowTimer(const float InRate,const FGameplayTagContainer& InTags);
	
};
