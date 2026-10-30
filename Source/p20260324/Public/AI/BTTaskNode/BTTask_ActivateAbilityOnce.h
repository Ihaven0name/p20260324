#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ActivateAbilityOnce.generated.h"

class UGameplayAbility;

/** 激活一次指定的 Gameplay Ability，然后立即结束任务。 */
UCLASS()
class P20260324_API UBTTask_ActivateAbilityOnce : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbilityOnce();

	/** 指定的能力必须已授予给受控 Pawn 的 ASC。 */
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	/** 激活能力前，从黑板读取并保存到敌人上的目标 Actor。 */
	UPROPERTY(EditAnywhere, Category = "Ability")
	FBlackboardKeySelector TargetActorKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
