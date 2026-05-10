#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

class UProjectAbilitySystemComponent;
struct FGameplayTagContainer;

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
	//添加InputWindowTimer,如果已经有了Timer，先进行清除，并且进行清理InputWindow对应的标签，给BaseCharacter里面的InputWindowTimerHandle进行赋值
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Anim")
	void AddInputWindowTimer(const float InRate,const FGameplayTagContainer& InTags);
	
};
