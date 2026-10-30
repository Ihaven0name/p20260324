// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_CheckObstacle.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UBTService_CheckObstacle::UBTService_CheckObstacle()
{
	NodeName = TEXT("Check Shooting Obstacle");
	Interval = 0.5f;
	bNotifyTick = true;

	SelfActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckObstacle, SelfActorKey), AActor::StaticClass());
	SelfActorKey.SelectedKeyName = TEXT("SelfActor");

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckObstacle, TargetActorKey), AActor::StaticClass());
	TargetActorKey.SelectedKeyName = TEXT("TargetActor");

	HasLineOfSightKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckObstacle, HasLineOfSightKey));
	HasLineOfSightKey.SelectedKeyName = TEXT("bHaveNoObstacle");

	InShootRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckObstacle, InShootRangeKey));
	InShootRangeKey.SelectedKeyName = TEXT("bInShootRange");
}

void UBTService_CheckObstacle::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	UpdateObstacleStatus(OwnerComp);
}

void UBTService_CheckObstacle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	UpdateObstacleStatus(OwnerComp);
}


void UBTService_CheckObstacle::UpdateObstacleStatus(UBehaviorTreeComponent& OwnerComp) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	AActor* SelfActor = Cast<AActor>(Blackboard->GetValueAsObject(SelfActorKey.SelectedKeyName));
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		// 没有目标时不处于受阻状态，默认标记为没有障碍。
		Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, true);
		Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, false);
		return;
	}

	if (!IsValid(SelfActor))
	{
		Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, false);
		return;
	}

	if (SocketName.IsNone())
	{
		Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, false);
		return;
	}

	USkeletalMeshComponent* Mesh = SelfActor->FindComponentByClass<USkeletalMeshComponent>();
	UWorld* World = SelfActor->GetWorld();
	if (!IsValid(Mesh) || !Mesh->DoesSocketExist(SocketName) || !World)
	{
		Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, false);
		return;
	}

	// 射线起点使用武器 Socket，方向与 Ability_Shoot 一样以角色 ForwardVector 为基准。
	const FVector TraceStart = Mesh->GetSocketLocation(SocketName);
	const FVector ShotDirection = SelfActor->GetActorForwardVector().GetSafeNormal();
	if (ShotDirection.IsNearlyZero())
	{
		Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, false);
		Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, false);
		return;
	}

	// 目标是否位于射击扇区内，以角色前向为基准；这只限制开火时机，不用于精准瞄准。
	FVector HorizontalShotDirection = ShotDirection;
	FVector HorizontalToTarget = TargetActor->GetActorLocation() - TraceStart;
	HorizontalShotDirection.Z = 0.0f;
	HorizontalToTarget.Z = 0.0f;
	const bool bHasValidDirections = !HorizontalShotDirection.IsNearlyZero() && !HorizontalToTarget.IsNearlyZero();
	const float HalfShootAngle = FMath::Clamp(ShootAngle, 0.0f, 360.0f) * 0.5f;
	const float DotThreshold = FMath::Cos(FMath::DegreesToRadians(HalfShootAngle));
	const bool bInShootAngle = bHasValidDirections
		&& FVector::DotProduct(HorizontalShotDirection.GetSafeNormal(), HorizontalToTarget.GetSafeNormal()) >= DotThreshold;
	Blackboard->SetValueAsBool(InShootRangeKey.SelectedKeyName, bInShootAngle);

	const float TraceDistance = FMath::Min(HorizontalToTarget.Size(), TraceRange);
	const FVector TraceEnd = TraceStart + ShotDirection * TraceDistance;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CheckShootingObstacle), true, SelfActor);
	FHitResult HitResult;
	const bool bBlockingHit = World->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		TraceChannel,
		QueryParams);

	// 目标或其他玩家命中不算环境障碍；其余阻挡说明当前枪口方向被遮挡。
	const AActor* HitActor = HitResult.GetActor();
	const bool bHitPlayer = IsValid(HitActor) && HitActor->ActorHasTag(FName(TEXT("Player")));
	const bool bHasLineOfSight = !bBlockingHit || HitActor == TargetActor || bHitPlayer;
	Blackboard->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, bHasLineOfSight);
}

