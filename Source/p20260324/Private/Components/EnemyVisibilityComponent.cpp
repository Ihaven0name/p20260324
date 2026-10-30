#include "Components/EnemyVisibilityComponent.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Interface/EnemyInterface.h"
#include "TimerManager.h"

UEnemyVisibilityComponent::UEnemyVisibilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyVisibilityComponent::BeginPlay()
{
	Super::BeginPlay();
	RefreshTrackingState();
}

void UEnemyVisibilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopTracking(true);
	Super::EndPlay(EndPlayReason);
}

void UEnemyVisibilityComponent::RefreshTrackingState()
{
	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
	UWorld* World = GetWorld();
	if (!World || !OwnerPlayerController || !OwnerPlayerController->IsLocalController() || !OwnerPlayerController->GetPawn())
	{
		StopTracking(true);
		return;
	}

	if (!World->GetTimerManager().IsTimerActive(UpdateTimerHandle))
	{
		World->GetTimerManager().SetTimer(
			UpdateTimerHandle,
			this,
			&ThisClass::UpdateVisibleEnemiesTimer,
			FMath::Max(UpdateInterval, 0.01f),
			true);
	}

	TArray<AActor*> IgnoredOutput;
	UpdateVisibleEnemies(IgnoredOutput);
}

void UEnemyVisibilityComponent::UpdateVisibleEnemies(TArray<AActor*>& OutVisibleEnemies)
{
	OutVisibleEnemies.Reset();
	APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
	APawn* PlayerPawn = OwnerPlayerController ? OwnerPlayerController->GetPawn() : nullptr;
	UWorld* World = GetWorld();
	if (!OwnerPlayerController || !OwnerPlayerController->IsLocalController() || !IsValid(PlayerPawn) || !World || !GEngine || !GEngine->GameViewport)
	{
		return;
	}

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	if (ViewportSize.X <= 0.0f || ViewportSize.Y <= 0.0f)
	{
		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	OwnerPlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	const FVector ViewForward = ViewRotation.Vector();
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	TSet<AActor*> CurrentVisibleSet;

	for (TActorIterator<AEnemyCharacter> It(World); It; ++It)
	{
		AEnemyCharacter* Enemy = *It;
		if (!IsValid(Enemy) || !Enemy->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			continue;
		}

		FVector BoundsOrigin;
		FVector BoundsExtent;
		Enemy->GetActorBounds(true, BoundsOrigin, BoundsExtent);
		(void)BoundsExtent;
		const FVector DirectionToEnemy = (BoundsOrigin - ViewLocation).GetSafeNormal();
		FVector2D ScreenPosition;
		if (FVector::DotProduct(ViewForward, DirectionToEnemy) <= 0.0f ||
			!OwnerPlayerController->ProjectWorldLocationToScreen(BoundsOrigin, ScreenPosition, true) ||
			ScreenPosition.X < 0.0f || ScreenPosition.X > ViewportSize.X ||
			ScreenPosition.Y < 0.0f || ScreenPosition.Y > ViewportSize.Y ||
			!OwnerPlayerController->LineOfSightTo(Enemy, ViewLocation, false))
		{
			continue;
		}

		CurrentVisibleSet.Add(Enemy);
		OutVisibleEnemies.Add(Enemy);
	}

	for (const TWeakObjectPtr<AActor>& Previous : PreviousVisibleEnemies)
	{
		AActor* Enemy = Previous.Get();
		if (IsValid(Enemy) && !CurrentVisibleSet.Contains(Enemy) &&
			Enemy->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			IEnemyInterface::Execute_UpdateHealthBarByPlayerLocation(Enemy, PlayerLocation, false);
		}
	}

	for (AActor* Enemy : CurrentVisibleSet)
	{
		IEnemyInterface::Execute_UpdateHealthBarByPlayerLocation(Enemy, PlayerLocation, true);
	}

	PreviousVisibleEnemies.Reset();
	PreviousVisibleEnemies.Reserve(CurrentVisibleSet.Num());
	for (AActor* Enemy : CurrentVisibleSet)
	{
		PreviousVisibleEnemies.Add(Enemy);
	}
}

void UEnemyVisibilityComponent::UpdateVisibleEnemiesTimer()
{
	TArray<AActor*> IgnoredOutput;
	UpdateVisibleEnemies(IgnoredOutput);
}

void UEnemyVisibilityComponent::StopTracking(const bool bHidePrevious)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(UpdateTimerHandle);
	}

	if (bHidePrevious)
	{
		const APlayerController* OwnerPlayerController = Cast<APlayerController>(GetOwner());
		const APawn* PlayerPawn = OwnerPlayerController ? OwnerPlayerController->GetPawn() : nullptr;
		HidePreviousEnemies(PlayerPawn ? PlayerPawn->GetActorLocation() : FVector::ZeroVector);
	}
	PreviousVisibleEnemies.Reset();
}

void UEnemyVisibilityComponent::HidePreviousEnemies(const FVector& PlayerLocation)
{
	for (const TWeakObjectPtr<AActor>& Previous : PreviousVisibleEnemies)
	{
		AActor* Enemy = Previous.Get();
		if (IsValid(Enemy) && Enemy->GetClass()->ImplementsInterface(UEnemyInterface::StaticClass()))
		{
			IEnemyInterface::Execute_UpdateHealthBarByPlayerLocation(Enemy, PlayerLocation, false);
		}
	}
}
