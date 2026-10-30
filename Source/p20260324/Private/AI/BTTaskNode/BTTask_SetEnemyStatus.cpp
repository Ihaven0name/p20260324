#include "AI/BTTaskNode/BTTask_SetEnemyStatus.h"

#include "AIController.h"
#include "Enemy/EnemyCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_SetEnemyStatus::UBTTask_SetEnemyStatus()
{
	NodeName = TEXT("Set Enemy Status");
}

EBTNodeResult::Type UBTTask_SetEnemyStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AEnemyCharacter* EnemyCharacter = AIController
		? Cast<AEnemyCharacter>(AIController->GetPawn())
		: nullptr;

	if (!EnemyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	EnemyCharacter->SetIsCombating(bIsCombating);
	return EBTNodeResult::Succeeded;
}
