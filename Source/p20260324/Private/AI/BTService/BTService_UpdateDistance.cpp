// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_UpdateDistance.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Interface/EnemyInterface.h"

UBTService_UpdateDistance::UBTService_UpdateDistance()
{
	NodeName = TEXT("Update Distance To Target");
	Interval = 0.1f;
	bNotifyTick = true;

	SelfActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateDistance, SelfActorKey), AActor::StaticClass());
	SelfActorKey.SelectedKeyName = TEXT("SelfActor");

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateDistance, TargetActorKey), AActor::StaticClass());
	TargetActorKey.SelectedKeyName = TEXT("TargetActor");

	DistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateDistance, DistanceKey));
	DistanceKey.SelectedKeyName = TEXT("DistanceToTargetActor");

	LostTargetKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateDistance, LostTargetKey));
	LostTargetKey.SelectedKeyName = TEXT("bLostTarget");

	TimeToLostTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateDistance, TimeToLostTargetKey));
	TimeToLostTargetKey.SelectedKeyName = TEXT("TimeToLostTarget");
}

void UBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	AActor* SelfActor = Cast<AActor>(Blackboard->GetValueAsObject(SelfActorKey.SelectedKeyName));
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(SelfActor) || !IsValid(TargetActor))
	{
		Blackboard->SetValueAsFloat(DistanceKey.SelectedKeyName, TNumericLimits<float>::Max());
		Blackboard->SetValueAsBool(LostTargetKey.SelectedKeyName, false);
		Blackboard->SetValueAsFloat(TimeToLostTargetKey.SelectedKeyName, 0.0f);
		return;
	}

	const float Distance = FVector::Distance(SelfActor->GetActorLocation(), TargetActor->GetActorLocation());
	Blackboard->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);

	if (Distance > LostTargetDistance)
	{
		Blackboard->SetValueAsBool(LostTargetKey.SelectedKeyName, true);
		const float TimeToLostTarget = Blackboard->GetValueAsFloat(TimeToLostTargetKey.SelectedKeyName) + DeltaSeconds;
		Blackboard->SetValueAsFloat(TimeToLostTargetKey.SelectedKeyName, TimeToLostTarget);

		if (TimeToLostTarget >= LostTargetDuration)
		{
			if (APawn* EnemyPawn = Cast<APawn>(SelfActor);
				IsValid(EnemyPawn) && EnemyPawn->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
			{
				IEnemyInterface::Execute_SetTargetActor(EnemyPawn, nullptr);
			}

			Blackboard->ClearValue(TargetActorKey.SelectedKeyName);
			Blackboard->SetValueAsBool(LostTargetKey.SelectedKeyName, false);
			Blackboard->SetValueAsFloat(TimeToLostTargetKey.SelectedKeyName, 0.0f);
		}
	}
	else
	{
		Blackboard->SetValueAsBool(LostTargetKey.SelectedKeyName, false);
		Blackboard->SetValueAsFloat(TimeToLostTargetKey.SelectedKeyName, 0.0f);
	}
}
