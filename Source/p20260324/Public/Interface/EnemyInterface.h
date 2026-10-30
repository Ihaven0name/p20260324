// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

class AActor;

// 此类无需额外修改。
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class P20260324_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** 设置敌人当前追踪的目标。 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|AI")
	void SetTargetActor(AActor* NewTargetActor);

	/** 获取敌人当前追踪的目标。 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|AI")
	AActor* GetTargetActor() const;

	/** 根据玩家位置和可见状态更新敌人血条的距离缩放与显示状态。 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy|UI")
	void UpdateHealthBarByPlayerLocation(FVector PlayerLocation, bool bIsVisible);
};
