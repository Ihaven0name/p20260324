#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class P20260324_API IPlayerInterface
{
	GENERATED_BODY()
public:
	// ============ 预输入相关 ========================
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool GetCanPreInput();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetCanPreInput(const bool InbCanPreInput);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void GetPreInputTag(TArray<FGameplayTag>& OutPreInputTag);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool FindActivatableAbilityTag();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddAbilityTag();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ClearPreInputTag();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Weapon")
	FName GetLeftHandSocketName();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Weapon")
	FName GetRightHandSocketName();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool GetbIsLocking();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetLockingActor();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	EPlayerCategory GetPlayerCategory();
};
