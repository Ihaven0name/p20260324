
#include "Enemy/EnemyController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy/EnemyCharacter.h"
#include "Interface/EnemyInterface.h"
#include "ProjectBaseCharacter.h"
#include "Perception/AIPerceptionComponent.h"

AEnemyController::AEnemyController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BrainComponent = BehaviorTreeComponent;
	// 行为树由 OnPossess 中的 RunBehaviorTree 明确启动，避免组件自动启动与之重复。
	bStartAILogicOnPossess = false;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(
		this, &AEnemyController::HandleTargetPerceptionUpdated);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyController: BehaviorTreeAsset is not assigned."));
		return;
	}

	if (!RunBehaviorTree(BehaviorTreeAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyController: Failed to run behavior tree '%s'."), *GetNameSafe(BehaviorTreeAsset));
	}
}

void AEnemyController::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!HasAuthority() || !IsValid(Actor))
	{
		return;
	}
	APawn* ControlledPawn = GetPawn();
	const AProjectBaseCharacter* EnemyCharacter = Cast<AProjectBaseCharacter>(ControlledPawn);
	if (!IsValid(EnemyCharacter) || EnemyCharacter->bIsDead ||
		!ControlledPawn->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		if (Actor->ActorHasTag(TEXT("Player")))
		{
			if (const AProjectBaseCharacter* PlayerCharacter = Cast<AProjectBaseCharacter>(Actor);
				PlayerCharacter && PlayerCharacter->bIsDead)
			{
				AIPerceptionComponent->ForgetActor(Actor);
				if (IEnemyInterface::Execute_GetTargetActor(ControlledPawn) == Actor)
				{
					IEnemyInterface::Execute_SetTargetActor(ControlledPawn, nullptr);
					if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ControlledPawn))
					{
						Enemy->SetAIState(false, false, true);
					}
				}
				return;
			}
			IEnemyInterface::Execute_SetTargetActor(ControlledPawn, Actor);
		}
	}
}
