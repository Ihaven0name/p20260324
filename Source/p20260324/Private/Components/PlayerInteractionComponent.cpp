#include "Components/PlayerInteractionComponent.h"

#include "Components/InteractableComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Interface/InteractableInterface.h"
#include "TimerManager.h"

UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	RefreshScanningState();
}

void UPlayerInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopScanning();
	Super::EndPlay(EndPlayReason);
}

void UPlayerInteractionComponent::RefreshScanningState()
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	if (!World || !OwnerPawn || !OwnerPawn->IsLocallyControlled())
	{
		StopScanning();
		return;
	}

	if (!World->GetTimerManager().IsTimerActive(InteractableScanTimerHandle))
	{
		World->GetTimerManager().SetTimer(
			InteractableScanTimerHandle,
			this,
			&ThisClass::ScanForInteractables,
			FMath::Max(InteractionScanInterval, 0.01f),
			true);
	}
	ScanForInteractables();
}

void UPlayerInteractionComponent::StopScanning()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(InteractableScanTimerHandle);
	}
	DetectedInteractableActors.Reset();
	SetCurrentTarget(nullptr);
}

void UPlayerInteractionComponent::ScanForInteractables()
{
	DetectedInteractableActors.Reset();
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UWorld* World = GetWorld();
	if (!World || !OwnerPawn || !OwnerPawn->IsLocallyControlled() || InteractionDetectionRadius <= 0.0f)
	{
		SetCurrentTarget(nullptr);
		return;
	}

	const FVector DetectionOrigin = OwnerPawn->GetActorLocation();
	const FVector ViewForward = OwnerPawn->GetViewRotation().Vector().GetSafeNormal();
	const float HalfAngleRadians = FMath::DegreesToRadians(InteractionConeAngleDegrees * 0.5f);
	const float MinimumForwardDot = FMath::Cos(HalfAngleRadians);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ScanForInteractables), false, OwnerPawn);
	QueryParams.AddIgnoredActor(OwnerPawn);

	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(
		Overlaps,
		DetectionOrigin,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(InteractionDetectionRadius),
		QueryParams);

	TSet<AActor*> SeenActors;
	TArray<AActor*> Candidates;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* Candidate = Overlap.GetActor();
		if (!IsValid(Candidate) || SeenActors.Contains(Candidate) || !CanInteractWithActor(Candidate))
		{
			continue;
		}

		const FVector ToCandidate = Candidate->GetActorLocation() - DetectionOrigin;
		const float DistanceSquared = ToCandidate.SizeSquared();
		if (DistanceSquared > FMath::Square(InteractionDetectionRadius) || DistanceSquared <= UE_SMALL_NUMBER ||
			FVector::DotProduct(ViewForward, ToCandidate.GetSafeNormal()) < MinimumForwardDot)
		{
			continue;
		}

		SeenActors.Add(Candidate);
		Candidates.Add(Candidate);
	}

	Candidates.Sort([&DetectionOrigin](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared(DetectionOrigin, A.GetActorLocation()) <
			FVector::DistSquared(DetectionOrigin, B.GetActorLocation());
	});
	for (AActor* Candidate : Candidates)
	{
		DetectedInteractableActors.Add(Candidate);
	}

	// 目标仍在半径内且有效时保持选中，不因短暂离开扇形或新目标出现而跳转。
	if (!CanInteractWithActor(CurrentInteractableActor))
	{
		SetCurrentTarget(DetectedInteractableActors.IsEmpty() ? nullptr : DetectedInteractableActors[0].Get());
	}
}

void UPlayerInteractionComponent::CycleTarget(const int32 Direction)
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled() || Direction == 0)
	{
		return;
	}

	TArray<AActor*> ValidCandidates;
	ValidCandidates.Reserve(DetectedInteractableActors.Num());
	for (const TObjectPtr<AActor>& CandidateObject : DetectedInteractableActors)
	{
		AActor* Candidate = CandidateObject.Get();
		if (CanInteractWithActor(Candidate))
		{
			ValidCandidates.AddUnique(Candidate);
		}
	}

	if (ValidCandidates.IsEmpty())
	{
		return;
	}

	const int32 CurrentIndex = ValidCandidates.IndexOfByKey(GetCurrentTarget());
	const int32 Step = Direction > 0 ? 1 : -1;
	const int32 NextIndex = CurrentIndex == INDEX_NONE
		? (Step > 0 ? 0 : ValidCandidates.Num() - 1)
		: (CurrentIndex + Step + ValidCandidates.Num()) % ValidCandidates.Num();
	SetCurrentTarget(ValidCandidates[NextIndex]);
}

AActor* UPlayerInteractionComponent::GetCurrentTarget() const
{
	return IsValid(CurrentInteractableActor) ? CurrentInteractableActor.Get() : nullptr;
}

FText UPlayerInteractionComponent::GetCurrentTargetDisplayName() const
{
	AActor* Target = GetCurrentTarget();
	if (!IsValid(Target))
	{
		return FText::GetEmpty();
	}

	if (const UInteractableComponent* InteractableComponent = Target->FindComponentByClass<UInteractableComponent>();
		IsValid(InteractableComponent) && !InteractableComponent->InteractionDisplayName.IsEmpty())
	{
		return InteractableComponent->InteractionDisplayName;
	}
	return FText::FromString(Target->GetName());
}

bool UPlayerInteractionComponent::CanInteractWithActor(const AActor* Target) const
{
	AActor* OwnerActor = GetOwner();
	const UInteractableComponent* InteractableComponent = IsValid(Target)
		? Target->FindComponentByClass<UInteractableComponent>()
		: nullptr;
	return IsValid(OwnerActor) && IsValid(Target) && Target->GetWorld() == OwnerActor->GetWorld() &&
		FVector::DistSquared(OwnerActor->GetActorLocation(), Target->GetActorLocation()) <= FMath::Square(InteractionDetectionRadius) &&
		IsValid(InteractableComponent) && InteractableComponent->CanInteractWith(OwnerActor);
}

bool UPlayerInteractionComponent::TryInteractWithActor(AActor* Target) const
{
	if (!CanInteractWithActor(Target))
	{
		return false;
	}

	UInteractableComponent* InteractableComponent = Target->FindComponentByClass<UInteractableComponent>();
	return IsValid(InteractableComponent) && InteractableComponent->TryInteract(GetOwner());
}

void UPlayerInteractionComponent::SetCurrentTarget(AActor* NewTarget)
{
	if (CurrentInteractableActor.Get() == NewTarget)
	{
		return;
	}

	AActor* PreviousTarget = CurrentInteractableActor.Get();
	CurrentInteractableActor = NewTarget;
	OnTargetChanged.Broadcast(PreviousTarget, NewTarget);
}
