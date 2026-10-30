// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckObstacle.generated.h"

/**
 * 
 */
UCLASS()
class P20260324_API UBTService_CheckObstacle : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckObstacle();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Service 激活时先立即计算一次，之后每个检测周期重复计算。 */
	void UpdateObstacleStatus(UBehaviorTreeComponent& OwnerComp) const;

	/** 黑板中保存敌人自身 Actor 的键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector SelfActorKey;

	/** 黑板中保存当前射击目标的键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector TargetActorKey;

	/** 用于写入射击视线是否畅通的 Bool 键。畅通为 true，被阻挡为 false。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector HasLineOfSightKey;

	/** 用于写入目标是否处于射击角度内的 Bool 键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector InShootRangeKey;

	/** 总射击角度，左右各占一半。例如 90 度表示正前方左右各 45 度。 */
	UPROPERTY(EditAnywhere, Category = "射击角度", meta = (ClampMin = "0.0", ClampMax = "360.0", Units = "deg"))
	float ShootAngle = 90.0f;

	/** 射线起点使用的角色网格 Socket，需与 Ability_Shoot 的 SocketName 一致。 */
	UPROPERTY(EditAnywhere, Category = "障碍检测")
	FName SocketName;

	/** 障碍检测使用的碰撞通道，需与 Ability_Shoot 的 TraceChannel 一致。 */
	UPROPERTY(EditAnywhere, Category = "障碍检测")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel1;

	/** 前向障碍检测距离，单位为厘米；建议与 Ability_Shoot 的 TraceRange 一致。 */
	UPROPERTY(EditAnywhere, Category = "障碍检测", meta = (ClampMin = "1.0", Units = "cm"))
	float TraceRange = 10000.0f;
};
