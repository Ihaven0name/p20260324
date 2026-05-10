// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyManager.generated.h"

class AActor;

/**
 * Enemy Manager - 管理游戏中的敌人
 */
UCLASS(BlueprintType, Blueprintable)
class P20260324_API UEnemyManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// 销毁
	virtual void Deinitialize() override;

	/*
	 * 生成敌人
	 * @param WorldContextObject 世界上下文对象
	 * @param EnemyClass 敌人蓝图类
	 * @param Location 生成位置
	 * @param Rotation 生成旋转
	 * @return 生成成功的敌人Actor，失败返回nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyManager", meta = (WorldContext = "WorldContextObject"))
	AActor* SpawnEnemy(UObject* WorldContextObject, TSubclassOf<AActor> EnemyClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	void AddEnemy(AActor* Enemy);

	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	void RemoveEnemy(AActor* Enemy);


	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	void ClearAllEnemies();


	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	TArray<AActor*> GetSpawnedEnemies() const;


	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	int32 GetEnemyCount() const;

	/**
	 * 获取屏幕内可见的敌人
	 * @param PlayerActor 玩家Actor，用于获取相机
	 * @param MaxDistance 最大检测距离
	 * @param VisibleRange 视野范围（角度），如90表示左右各45度
	 * @return 屏幕内的敌人数组
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	TArray<AActor*> GetVisibleEnemies(AActor* PlayerActor, float MaxDistance = 1500.0f, float VisibleRange = 90.0f);

	/**
	 * 获取指定方向侧的敌人
	 * @param PlayerActor 玩家Actor
	 * @param CurrentTarget 当前锁定的敌人
	 * @param bIsRight true为右侧，false为左侧
	 * @param VisibleRange 视野范围（角度）
	 * @return 找到的敌人，如果没有则返回nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyManager")
	AActor* GetSideEnemy(AActor* PlayerActor, AActor* CurrentTarget, bool bIsRight, float VisibleRange = 90.0f);

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedEnemies;
};
