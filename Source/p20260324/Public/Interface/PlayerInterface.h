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
	/** 收集当前屏幕中可见且实现敌人接口的敌人，并更新其血条显示与距离缩放。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="UI|敌人血条")
	void UpdateVisibleEnemyHealthBars(TArray<AActor*>& OutVisibleEnemies);

	/** 扫描玩家前方扇形范围内可交互的对象，并按距离从近到远返回。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="交互")
	void DetectInteractableActors(TArray<AActor*>& OutInteractableActors);

	/** 返回当前锁定的交互对象；只要它仍在有效范围内，就不会因新对象出现而切换。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="交互")
	AActor* GetCurrentInteractableActor();

	/** 返回当前交互对象的显示名称。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="交互")
	FText GetCurrentInteractableDisplayName();

	/** 对当前交互对象调用其交互接口。 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="交互")
	void InteractWithCurrentTarget();
};
