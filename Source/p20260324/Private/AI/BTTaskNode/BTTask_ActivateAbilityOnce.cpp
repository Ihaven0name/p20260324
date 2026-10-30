#include "AI/BTTaskNode/BTTask_ActivateAbilityOnce.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Interface/EnemyInterface.h"

UBTTask_ActivateAbilityOnce::UBTTask_ActivateAbilityOnce()
{
	NodeName = TEXT("Activate Ability Once");
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ActivateAbilityOnce, TargetActorKey), AActor::StaticClass());
	TargetActorKey.SelectedKeyName = TEXT("TargetActor");
}

EBTNodeResult::Type UBTTask_ActivateAbilityOnce::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
	if (!ControlledPawn || !ControlledPawn->HasAuthority() || !AbilityClass)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Blackboard
		? Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName))
		: nullptr;
	if (!TargetActor || !ControlledPawn->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activate Ability Once: %s has no AbilitySystemComponent."), *GetNameSafe(ControlledPawn));
		return EBTNodeResult::Failed;
	}

	const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!AbilitySpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activate Ability Once: %s has not been granted to %s."), *GetNameSafe(AbilityClass), *GetNameSafe(ControlledPawn));
		return EBTNodeResult::Failed;
	}

	IEnemyInterface::Execute_SetTargetActor(ControlledPawn, TargetActor);
	const bool bActivated = ASC->TryActivateAbility(AbilitySpec->Handle);

	return bActivated ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
