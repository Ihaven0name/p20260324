
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

class UBehaviorTreeComponent;
class UBehaviorTree;
class UAIPerceptionComponent;

UCLASS()
class P20260324_API AEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyController();

	/** Pawn 被控制器接管后，启动配置的行为树并初始化其黑板。 */
	virtual void OnPossess(APawn* InPawn) override;

	/** 根据感知结果，将带有 Player Actor Tag 的目标交给敌人接口处理。 */
	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


	/** 负责执行行为树的组件。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	/** 此控制器接管 Pawn 后运行的行为树资产。 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Behavior Tree")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	/** 负责感知视觉、听觉等刺激的组件。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

};
