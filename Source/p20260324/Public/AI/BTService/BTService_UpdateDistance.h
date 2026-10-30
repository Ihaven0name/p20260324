// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateDistance.generated.h"

UCLASS()
class P20260324_API UBTService_UpdateDistance : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_UpdateDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** 黑板中保存敌人自身的 Actor 键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector SelfActorKey;

	/** 黑板中保存当前目标的 Actor 键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector TargetActorKey;

	/** 用于写入敌人与目标之间距离的 Float 键，单位为厘米。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector DistanceKey;

	/** 用于标记目标是否超出丢失距离的 Bool 键。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector LostTargetKey;

	/** 累计目标超出丢失距离的时间，单位为秒。 */
	UPROPERTY(EditAnywhere, Category = "黑板")
	FBlackboardKeySelector TimeToLostTargetKey;

	/** 目标超过此距离后开始累计丢失时间，单位为厘米。 */
	UPROPERTY(EditAnywhere, Category = "目标丢失", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LostTargetDistance = 2000.0f;

	/** 目标持续超过丢失距离多久后清空目标，单位为秒。 */
	UPROPERTY(EditAnywhere, Category = "目标丢失", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LostTargetDuration = 5.0f;
};
