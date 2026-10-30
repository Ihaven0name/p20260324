// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTargetActor.generated.h"

class AAIController;
class ACharacter;
class AEnemyCharacter;
class UBlendSpace;
class UCharacterMovementComponent;
class UProjectAnimInstance;

/**
 * Turns a character toward the selected blackboard actor while its AnimBP plays
 * the configured turn BlendSpace.
 */
UCLASS()
class P20260324_API UBTTask_TurnToTargetActor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTargetActor();

	UPROPERTY(EditAnywhere, Category = "Turn")
	TObjectPtr<UBlendSpace> TurnBlendSpace;

	UPROPERTY(EditAnywhere, Category = "Turn")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Turn", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float StartAngleTolerance = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Turn", meta = (ClampMin = "0.01", UIMin = "0.01", Units = "s"))
	float BaseTurnDuration = 0.25f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool CalculateTurnPlayback(float SignedAngle, float& OutSignedRotationSpeed, float& OutDuration) const;
	void FinishTurn(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type Result);
	void RestoreTurnState();

	TWeakObjectPtr<AAIController> ActiveController;
	TWeakObjectPtr<ACharacter> ActiveCharacter;
	TWeakObjectPtr<AEnemyCharacter> ActiveEnemy;
	TWeakObjectPtr<UCharacterMovementComponent> ActiveMovement;
	TWeakObjectPtr<UProjectAnimInstance> ActiveAnimInstance;
	TWeakObjectPtr<AActor> ActiveTarget;
	TWeakObjectPtr<AActor> PreviousFocusActor;

	FRotator OriginalRotationRate = FRotator::ZeroRotator;
	bool bOriginalOrientRotationToMovement = false;
	bool bOriginalUseControllerDesiredRotation = false;
	bool bOriginalUseControllerRotationYaw = false;
	bool bHadPreviousFocus = false;
	bool bTurnInProgress = false;
	float TurnStartTime = 0.0f;
	float TurnAnimationDuration = 0.0f;
};
