#include "Components/TargetLockComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/EnemyManager.h"
#include "ProjectGameInstance.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectPlayerController.h"

UTargetLockComponent::UTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UTargetLockComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLockOnRotation(DeltaTime);
}

void UTargetLockComponent::ToggleLockOn()
{
	AProjectPlayerController* OwnerPlayerController = Cast<AProjectPlayerController>(GetOwner());
	AProjectPlayerCharacter* PlayerCharacter = OwnerPlayerController ? OwnerPlayerController->GetPlayerCharacter() : nullptr;
	if (!OwnerPlayerController || !OwnerPlayerController->IsLocalController() || !IsValid(PlayerCharacter))
	{
		return;
	}

	if (bIsLocking)
	{
		ResetLock();
		return;
	}

	UEnemyManager* EnemyManager = UProjectBlueprintFunctionLibrary::GetEnemyManager(OwnerPlayerController);
	if (!IsValid(EnemyManager))
	{
		return;
	}

	const TArray<AActor*> VisibleActors = EnemyManager->GetVisibleEnemies(PlayerCharacter, MaxLockDistance);
	AActor* NewTarget = UProjectBlueprintFunctionLibrary::FindNearestActor(PlayerCharacter, VisibleActors);
	if (!IsValid(NewTarget))
	{
		return;
	}

	LockedPawn = PlayerCharacter;
	PlayerCharacter->bUseControllerRotationYaw = true;
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	CurrentControlRotation = OwnerPlayerController->GetControlRotation();
	SetLockTarget(NewTarget);
}

void UTargetLockComponent::AddTargetSwitchInput(const float AxisValue)
{
	AProjectPlayerController* OwnerPlayerController = Cast<AProjectPlayerController>(GetOwner());
	AProjectPlayerCharacter* PlayerCharacter = OwnerPlayerController ? OwnerPlayerController->GetPlayerCharacter() : nullptr;
	if (!bIsLocking || !IsValid(LockingActor) || !OwnerPlayerController || !IsValid(PlayerCharacter))
	{
		return;
	}

	const UProjectGameInstance* GameInstance = OwnerPlayerController->GetProjectGameInstance();
	if (!IsValid(GameInstance))
	{
		return;
	}

	SwitchTargetInput += AxisValue * GameInstance->LockOnSwitchSensitivity;
	if (FMath::Abs(SwitchTargetInput) <= SwitchTargetThreshold)
	{
		return;
	}

	const bool bSwitchRight = SwitchTargetInput > 0.0f;
	SwitchTargetInput = 0.0f;
	UEnemyManager* EnemyManager = UProjectBlueprintFunctionLibrary::GetEnemyManager(OwnerPlayerController);
	if (!IsValid(EnemyManager))
	{
		return;
	}

	if (AActor* NewTarget = EnemyManager->GetSideEnemy(PlayerCharacter, LockingActor, bSwitchRight, MaxLockAngle))
	{
		SetLockTarget(NewTarget);
	}
}

void UTargetLockComponent::UpdateLockOnRotation(const float DeltaTime)
{
	if (!bIsLocking)
	{
		return;
	}

	AProjectPlayerController* OwnerPlayerController = Cast<AProjectPlayerController>(GetOwner());
	AProjectPlayerCharacter* PlayerCharacter = OwnerPlayerController ? OwnerPlayerController->GetPlayerCharacter() : nullptr;
	if (!OwnerPlayerController || !IsValid(PlayerCharacter) || !IsValid(LockingActor) || LockedPawn.Get() != PlayerCharacter)
	{
		ResetLock();
		return;
	}

	FRotator TargetRotation = (LockingActor->GetActorLocation() - PlayerCharacter->GetActorLocation()).Rotation();
	TargetRotation.Pitch = LockOnPitchRadius;
	TargetRotation.Normalize();
	CurrentControlRotation = FMath::RInterpTo(CurrentControlRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
	OwnerPlayerController->SetControlRotation(CurrentControlRotation);
}

AActor* UTargetLockComponent::GetLockingActor() const
{
	return IsValid(LockingActor) ? LockingActor.Get() : nullptr;
}

void UTargetLockComponent::ResetLock()
{
	if (ACharacter* PreviousCharacter = Cast<ACharacter>(LockedPawn.Get()))
	{
		PreviousCharacter->bUseControllerRotationYaw = false;
		if (UCharacterMovementComponent* Movement = PreviousCharacter->GetCharacterMovement())
		{
			Movement->bOrientRotationToMovement = true;
		}
	}

	LockedPawn.Reset();
	SwitchTargetInput = 0.0f;
	SetLockTarget(nullptr);
}

void UTargetLockComponent::SetLockTarget(AActor* NewTarget)
{
	LockingActor = NewTarget;
	bIsLocking = IsValid(NewTarget);
	SetComponentTickEnabled(bIsLocking);
	if (!bIsLocking)
	{
		LockedPawn.Reset();
		SwitchTargetInput = 0.0f;
	}
}
