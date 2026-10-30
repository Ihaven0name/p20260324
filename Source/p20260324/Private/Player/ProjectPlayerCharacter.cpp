

#include "Player/ProjectPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "Camera/CameraComponent.h"
#include "Components/EnemyVisibilityComponent.h"
#include "Components/PlayerInteractionComponent.h"
#include "Components/TargetLockComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/ConfigManager.h"
#include "Manager/UIManager.h"
#include "Player/ProjectPlayerController.h"
#include "Player/ProjectPlayerState.h"
#include "UI/HUD/ProjectAttackHUD.h"
#include "Engine/World.h"
#include "Components/InteractableComponent.h"
#include "GameFramework/PlayerController.h"
#include "GAS/ProjectGameplayTag.h"
#include "TimerManager.h"

AProjectPlayerCharacter::AProjectPlayerCharacter()
{
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength=500.f;
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation=true;
	SpringArmComponent->bInheritPitch=true;
	SpringArmComponent->bInheritRoll=true;
	SpringArmComponent->bInheritYaw=true;
	
	FRotator Rotator=FRotator(-25.0f,0.f,0.0f);
	SpringArmComponent->SetRelativeRotation(Rotator);
	
	CameraComponent=CreateDefaultSubobject<UCameraComponent>("CameraComponent");	
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation=false;
	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>("PlayerInteractionComponent");
	PlayerInteractionComponent->OnTargetChanged.AddDynamic(this, &ThisClass::HandleInteractionTargetChanged);

	// ASC
	AbilitySystemComponent = CreateDefaultSubobject<UProjectAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UProjectAttributeSet>("AttributeSet");
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());
	
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement=true;
	CharacterMovementComponent->RotationRate=FRotator(0.f, 400.f, 0.f);
	CharacterMovementComponent->bConstrainToPlane = true;
	CharacterMovementComponent->bSnapToPlaneAtStart = true;
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
}

void AProjectPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerInteractionComponent) PlayerInteractionComponent->RefreshScanningState();
}

void AProjectPlayerCharacter::OnDeathStateChanged()
{
	Super::OnDeathStateChanged();

	if (AProjectPlayerController* PlayerController = GetProjectPlayerController())
	{
		PlayerController->bCanPreInput = false;
		PlayerController->PreInputTags.Reset();
	}

	if (GetNetMode() != NM_DedicatedServer)
	{
		ScheduleDeathWidget();
	}
}

void AProjectPlayerCharacter::ScheduleDeathWidget()
{
	if (!bIsDead || !GetWorld())
	{
		return;
	}

	if (DeathUIShowDelay <= 0.0f)
	{
		ShowDeathWidget();
		return;
	}

	GetWorldTimerManager().SetTimer(
		DeathUIShowTimerHandle,
		this,
		&ThisClass::ShowDeathWidget,
		DeathUIShowDelay,
		false);
}

void AProjectPlayerCharacter::ShowDeathWidget()
{
	if (!bIsDead || !IsLocallyControlled())
	{
		return;
	}

	if (AProjectPlayerController* PlayerController = GetProjectPlayerController())
	{
		if (UUIManager* UIManager = UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController))
		{
			UIManager->PushWidget(FProjectGameplayTag::Get().UI_Widget_MainAttack_Death);
		}
	}
}

void AProjectPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetOwner(NewController);
}

void AProjectPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (IsLocallyControlled())
	{
		if (AProjectPlayerController* PlayerController = GetProjectPlayerController())
		{
			PlayerController->SwitchCharacterIMC(PlayerCategory);
		}
		if (PlayerInteractionComponent) PlayerInteractionComponent->RefreshScanningState();
	}
	else
	{
		if (PlayerInteractionComponent) PlayerInteractionComponent->RefreshScanningState();
	}
}

void AProjectPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	SetOwner(GetController());
	if (PlayerInteractionComponent) PlayerInteractionComponent->RefreshScanningState();
	if (bIsDead && IsLocallyControlled())
	{
		ScheduleDeathWidget();
	}
}

void AProjectPlayerCharacter::InitAbilityActorInfo()
{
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	
	// 只有服务端才 Give Ability（客户端通过 Replication 自动同步 Spec）
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->GiveAbilitiesFromInputInfo();
		// 初始化 Startup Abilities（仅服务器 Give，Mixed 模式下客户端自动同步）
		UProjectBlueprintFunctionLibrary::InitializePlayerStartupAbilities(GetAbilitySystemComponent(), PlayerCategory);
	}
	// 初始化属性 GE（MMC + Override）—— 两端都需要初始化本地属性
	UProjectBlueprintFunctionLibrary::InitializePlayerAttributeByEffect(GetAbilitySystemComponent(), PlayerCategory);
	
}

void AProjectPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

bool AProjectPlayerCharacter::GetCanPreInput_Implementation()
{
	return GetProjectPlayerController()->bCanPreInput;
}

void AProjectPlayerCharacter::SetCanPreInput_Implementation(const bool bPreInput)
{
	GetProjectPlayerController()->bCanPreInput = bPreInput;
}

void AProjectPlayerCharacter::GetPreInputTag_Implementation(TArray<FGameplayTag>& OutPreInputTag)
{
	OutPreInputTag=GetProjectPlayerController()->PreInputTags;
}

bool AProjectPlayerCharacter::FindActivatableAbilityTag_Implementation()
{
	return GetProjectPlayerController()->FindActivatableAbilityTag();
}

void AProjectPlayerCharacter::ClearPreInputTag_Implementation()
{
	GetProjectPlayerController()->PreInputTags.Reset();
}

FName AProjectPlayerCharacter::GetLeftHandSocketName_Implementation()
{
	return LeftHandSocketName; 
}

FName AProjectPlayerCharacter::GetRightHandSocketName_Implementation()
{
	return RightHandSocketName;
}

bool AProjectPlayerCharacter::GetbIsLocking_Implementation()
{
	const AProjectPlayerController* OwnerPlayerController = GetProjectPlayerController();
	const UTargetLockComponent* LockComponent = OwnerPlayerController ? OwnerPlayerController->GetTargetLockComponent() : nullptr;
	return LockComponent ? LockComponent->IsLocking() : false;
}

AActor* AProjectPlayerCharacter::GetLockingActor_Implementation()
{
	const AProjectPlayerController* OwnerPlayerController = GetProjectPlayerController();
	const UTargetLockComponent* LockComponent = OwnerPlayerController ? OwnerPlayerController->GetTargetLockComponent() : nullptr;
	return LockComponent ? LockComponent->GetLockingActor() : nullptr;
}

EPlayerCategory AProjectPlayerCharacter::GetPlayerCategory_Implementation()
{
	return PlayerCategory;
}

void AProjectPlayerCharacter::UpdateVisibleEnemyHealthBars_Implementation(TArray<AActor*>& OutVisibleEnemies)
{
	OutVisibleEnemies.Reset();
	AProjectPlayerController* OwnerPlayerController = GetProjectPlayerController();
	UEnemyVisibilityComponent* VisibilityComponent = OwnerPlayerController ? OwnerPlayerController->GetEnemyVisibilityComponent() : nullptr;
	if (VisibilityComponent)
	{
		VisibilityComponent->UpdateVisibleEnemies(OutVisibleEnemies);
	}
}

void AProjectPlayerCharacter::DetectInteractableActors_Implementation(TArray<AActor*>& OutInteractableActors)
{
	OutInteractableActors.Reset();
	if (!PlayerInteractionComponent) return;

	PlayerInteractionComponent->ScanForInteractables();
	for (const TObjectPtr<AActor>& Candidate : PlayerInteractionComponent->DetectedInteractableActors)
	{
		OutInteractableActors.Add(Candidate.Get());
	}
}

AActor* AProjectPlayerCharacter::GetCurrentInteractableActor_Implementation()
{
	return PlayerInteractionComponent ? PlayerInteractionComponent->GetCurrentTarget() : nullptr;
}

void AProjectPlayerCharacter::CycleInteractableTarget(const int32 Direction)
{
	if (PlayerInteractionComponent)
	{
		PlayerInteractionComponent->CycleTarget(Direction);
	}
}

FText AProjectPlayerCharacter::GetCurrentInteractableDisplayName_Implementation()
{
	return PlayerInteractionComponent ? PlayerInteractionComponent->GetCurrentTargetDisplayName() : FText::GetEmpty();
}

void AProjectPlayerCharacter::InteractWithCurrentTarget_Implementation()
{
	AActor* Target = PlayerInteractionComponent ? PlayerInteractionComponent->GetCurrentTarget() : nullptr;
	if (!IsValid(Target)) return;

	if (HasAuthority()) Server_InteractWithTarget_Implementation(Target);
	else Server_InteractWithTarget(Target);
}

void AProjectPlayerCharacter::Server_InteractWithTarget_Implementation(AActor* Target)
{
	// 服务端再次校验距离和可交互状态，再通过目标的交互组件执行逻辑。
	if (PlayerInteractionComponent)
	{
		PlayerInteractionComponent->TryInteractWithActor(Target);
	}
}

void AProjectPlayerCharacter::HandleInteractionTargetChanged(AActor* PreviousTarget, AActor* NewTarget)
{
	OnInteractableTargetChanged.Broadcast(PreviousTarget, NewTarget);
}

AProjectPlayerController* AProjectPlayerCharacter::GetProjectPlayerController()
{
	if (ProjectPlayerController==nullptr)
	{
		ProjectPlayerController=Cast<AProjectPlayerController>(GetController());
	}
	return ProjectPlayerController;
}

AProjectPlayerState* AProjectPlayerCharacter::GetProjectPlayerState()
{
	if (ProjectPlayerState==nullptr)
	{
		ProjectPlayerState=Cast<AProjectPlayerState>(GetPlayerState());
	}
	return ProjectPlayerState;
}

