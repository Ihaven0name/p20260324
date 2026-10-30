// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetEnemyStatus.generated.h"

UCLASS()
class P20260324_API UBTTask_SetEnemyStatus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetEnemyStatus();

	UPROPERTY(EditAnywhere, Category = "Enemy Status")
	bool bIsCombating = false;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
