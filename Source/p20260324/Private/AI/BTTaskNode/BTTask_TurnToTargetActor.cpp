// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode/BTTask_TurnToTargetActor.h"

#include "AIController.h"
#include "Animation/BlendSpace.h"
#include "Animation/ProjectAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/EnemyCharacter.h"

UBTTask_TurnToTargetActor::UBTTask_TurnToTargetActor()
{
	NodeName = TEXT("Turn To Target Actor");
	bNotifyTick = true;
	bCreateNodeInstance = true;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_TurnToTargetActor, TargetActorKey), AActor::StaticClass());
	TargetActorKey.SelectedKeyName = TEXT("TargetActor");
}

EBTNodeResult::Type UBTTask_TurnToTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	ACharacter* Character = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;
	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Character);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Blackboard ? Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName)) : nullptr;
	UCharacterMovementComponent* Movement = Character ? Character->GetCharacterMovement() : nullptr;
	UProjectAnimInstance* AnimInstance = Character && Character->GetMesh()
		? Cast<UProjectAnimInstance>(Character->GetMesh()->GetAnimInstance())
		: nullptr;

	if (!AIController || !Character || !Enemy || !Character->HasAuthority() || !IsValid(TargetActor) ||
		!Movement || !AnimInstance || !TurnBlendSpace)
	{
		return EBTNodeResult::Failed;
	}

	FVector ToTarget = TargetActor->GetActorLocation() - Character->GetActorLocation();
	ToTarget.Z = 0.0f;
	if (ToTarget.IsNearlyZero())
	{
		return EBTNodeResult::Succeeded;
	}
	const float SignedAngle = FMath::FindDeltaAngleDegrees(
		Character->GetActorRotation().Yaw,
		ToTarget.Rotation().Yaw);
	const bool bShouldTurn = FMath::Abs(SignedAngle) > StartAngleTolerance;
	if (!bShouldTurn)
	{
		return EBTNodeResult::Succeeded;
	}

	float SignedRotationSpeed = 0.0f;
	if (!CalculateTurnPlayback(SignedAngle, SignedRotationSpeed, TurnAnimationDuration))
	{
		UE_LOG(LogTemp, Warning, TEXT("TurnToTargetActor: BlendSpace '%s' needs a non-negative RotationSpeed axis with valid samples; use the signed speed to mirror left turns in the AnimBP."), *GetNameSafe(TurnBlendSpace));
		return EBTNodeResult::Failed;
	}

	ActiveController = AIController;
	ActiveCharacter = Character;
	ActiveEnemy = Enemy;
	ActiveMovement = Movement;
	ActiveAnimInstance = AnimInstance;
	ActiveTarget = TargetActor;

	OriginalRotationRate = Movement->RotationRate;
	bOriginalOrientRotationToMovement = Movement->bOrientRotationToMovement;
	bOriginalUseControllerDesiredRotation = Movement->bUseControllerDesiredRotation;
	bOriginalUseControllerRotationYaw = Character->bUseControllerRotationYaw;
	PreviousFocusActor = AIController->GetFocusActorForPriority(EAIFocusPriority::Gameplay);
	bHadPreviousFocus = IsValid(PreviousFocusActor.Get());

	Movement->bOrientRotationToMovement = false;
	Movement->bUseControllerDesiredRotation = true;
	Movement->RotationRate.Yaw = FMath::Abs(SignedRotationSpeed);
	Character->bUseControllerRotationYaw = false;
	AIController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
	Enemy->SetTurnAnimationState(true, SignedRotationSpeed);

	TurnStartTime = Character->GetWorld()->GetTimeSeconds();
	bTurnInProgress = true;
	return EBTNodeResult::InProgress;
}

void UBTTask_TurnToTargetActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* TargetActor = ActiveTarget.Get();
	ACharacter* Character = ActiveCharacter.Get();
	if (!IsValid(TargetActor) || !IsValid(Character))
	{
		FinishTurn(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float Elapsed = Character->GetWorld()->GetTimeSeconds() - TurnStartTime;
	if (Elapsed >= TurnAnimationDuration)
	{
		FinishTurn(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_TurnToTargetActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	RestoreTurnState();
	return EBTNodeResult::Aborted;
}

bool UBTTask_TurnToTargetActor::CalculateTurnPlayback(
	const float SignedAngle,
	float& OutSignedRotationSpeed,
	float& OutDuration) const
{
	if (!TurnBlendSpace)
	{
		return false;
	}

	const FBlendParameter& SpeedParameter = TurnBlendSpace->GetBlendParameter(0);
	const float MinSpeed = FMath::Max(0.0f, SpeedParameter.Min);
	if (!FMath::IsFinite(BaseTurnDuration) || BaseTurnDuration <= KINDA_SMALL_NUMBER ||
		SpeedParameter.Min >= SpeedParameter.Max || !FMath::IsFinite(SpeedParameter.Max) || SpeedParameter.Max <= 0.0f)
	{
		return false;
	}

	const float AngleMagnitude = FMath::Abs(SignedAngle);
	const float MaxRotationSpeed = SpeedParameter.Max;
	float Speed = FMath::Clamp(AngleMagnitude / BaseTurnDuration, MinSpeed, MaxRotationSpeed);
	OutDuration = FMath::Max(BaseTurnDuration, AngleMagnitude / MaxRotationSpeed);
	int32 CachedTriangulationIndex = INDEX_NONE;
	TArray<FBlendSampleData> Samples;
	if (!TurnBlendSpace->GetSamplesFromBlendInput(FVector(Speed, 0.0f, 0.0f), Samples, CachedTriangulationIndex, true) || Samples.IsEmpty())
	{
		return false;
	}

	OutSignedRotationSpeed = FMath::Sign(SignedAngle) * Speed;
	return FMath::IsFinite(OutDuration) && OutDuration > KINDA_SMALL_NUMBER && FMath::IsFinite(OutSignedRotationSpeed);
}

void UBTTask_TurnToTargetActor::FinishTurn(UBehaviorTreeComponent& OwnerComp, const EBTNodeResult::Type Result)
{
	RestoreTurnState();
	FinishLatentTask(OwnerComp, Result);
}

void UBTTask_TurnToTargetActor::RestoreTurnState()
{
	if (!bTurnInProgress)
	{
		ActiveController.Reset();
		ActiveCharacter.Reset();
		ActiveEnemy.Reset();
		ActiveMovement.Reset();
		ActiveAnimInstance.Reset();
		ActiveTarget.Reset();
		return;
	}

	if (UProjectAnimInstance* AnimInstance = ActiveAnimInstance.Get())
	{
		AnimInstance->StopTurnToTarget();
	}
	if (AEnemyCharacter* Enemy = ActiveEnemy.Get())
	{
		Enemy->SetTurnAnimationState(false, 0.0f);
	}

	if (UCharacterMovementComponent* Movement = ActiveMovement.Get())
	{
		Movement->RotationRate = OriginalRotationRate;
		Movement->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
		Movement->bUseControllerDesiredRotation = bOriginalUseControllerDesiredRotation;
	}

	if (AAIController* AIController = ActiveController.Get())
	{
		if (bHadPreviousFocus && IsValid(PreviousFocusActor.Get()))
		{
			AIController->SetFocus(PreviousFocusActor.Get(), EAIFocusPriority::Gameplay);
		}
		else
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
		}
	}

	if (ACharacter* Character = ActiveCharacter.Get())
	{
		Character->bUseControllerRotationYaw = bOriginalUseControllerRotationYaw;
	}

	ActiveController.Reset();
	ActiveCharacter.Reset();
	ActiveEnemy.Reset();
	ActiveMovement.Reset();
	ActiveAnimInstance.Reset();
	ActiveTarget.Reset();
	PreviousFocusActor.Reset();
	bHadPreviousFocus = false;
	bTurnInProgress = false;
}

