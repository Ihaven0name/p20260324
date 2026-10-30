#include "Player/ProjectPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "ProjectBaseCharacter.h"
#include "ProjectGameInstance.h"
#include "Components/EnemyVisibilityComponent.h"
#include "Components/PlayerInteractionComponent.h"
#include "Components/TargetLockComponent.h"
#include "Manager/ConfigManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Info/InputInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/EnemyManager.h"
#include "Interface/EnemyInterface.h"
#include "Interface/PlayerInterface.h"
#include "Manager/InputManager.h"
#include "Abilities/GameplayAbility.h"
#include "Components/CapsuleComponent.h"
#include "GAS/ProjectGameplayTag.h"
#include "Input/ProjectInputComponent.h"
#include "Manager/UIManager.h"
#include "GameMode/PlayGameModeBase.h"
#include "Player/ProjectPlayerState.h"
#include "UI/HUD/ProjectAttackHUD.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "Manager/UIManager.h"

class UEnhancedInputLocalPlayerSubsystem;

AProjectPlayerController::AProjectPlayerController()
{
	TargetLockComponent = CreateDefaultSubobject<UTargetLockComponent>(TEXT("TargetLockComponent"));
	EnemyVisibilityComponent = CreateDefaultSubobject<UEnemyVisibilityComponent>(TEXT("EnemyVisibilityComponent"));
}


void AProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyVisibilityComponent)
	{
		EnemyVisibilityComponent->RefreshTrackingState();
	}

	if (UInputManager* InputManager = GetGameInstance()->GetSubsystem<UInputManager>())
	{
		InputManager->InitializeMappableByPlayerController(this);
		if (AProjectPlayerCharacter* CurrentPlayerCharacter = GetPlayerCharacter())
		{
			SwitchCharacterIMC(CurrentPlayerCharacter->PlayerCategory);
		}
		InputManager->ActivateMappedInputMappingContextByTag(FProjectGameplayTag::Get().MappableGroup_Primary, 0);
	}
	
	//TODO:在存档实现之后，需要实现存档加载角色小队
	// FInputModeGameAndUI InputModeData;
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	// SetInputMode(InputModeData);
	//影响LookActionFunction的执行
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
}

void AProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UProjectInputComponent* EnhancedInputComponent=CastChecked<UProjectInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ThisClass::MoveActionFunction);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ThisClass::LookActionFunction);
	EnhancedInputComponent->BindAction(WheelAction,ETriggerEvent::Triggered,this,&ThisClass::WheelActionFunction);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ThisClass::JumpActionFunction);
	EnhancedInputComponent->BindAction(LockOnAction,ETriggerEvent::Started,this,&ThisClass::LockOnActionFunction);
	EnhancedInputComponent->BindAction(LockSwitchAction,ETriggerEvent::Triggered,this,&ThisClass::LockSwitchActionFunction);

	// 绑定所有角色所有技能输入
	if (const UInputInfo* InputInfo = UProjectBlueprintFunctionLibrary::GetConfigManager(this)->GetInputInfo())
	{
		// 绑定 Judge Ability（每个 IA 的 Started/Completed 绑定到 Judge 回调）
		EnhancedInputComponent->BindJudgeActions(
			InputInfo,
			this,
			&ThisClass::OnAbilityJudgeStarted,
			&ThisClass::OnAbilityJudgeReleased
		);
		EnhancedInputComponent->BindAbilityActions(
			InputInfo,
			this,
			&ThisClass::OnAbilityInputStarted,
			&ThisClass::OnAbilityInputReleased
		);
	}
}

void AProjectPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AbilitySystemComponent = nullptr;
	if (TargetLockComponent) TargetLockComponent->ResetLock();
	if (EnemyVisibilityComponent) EnemyVisibilityComponent->RefreshTrackingState();
	RefreshMainAttackUIWidgetController();
}

void AProjectPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	AbilitySystemComponent = nullptr;
	if (TargetLockComponent) TargetLockComponent->ResetLock();
	if (EnemyVisibilityComponent) EnemyVisibilityComponent->RefreshTrackingState();
	RefreshMainAttackUIWidgetController();
}

void AProjectPlayerController::RefreshMainAttackUIWidgetController()
{
	if (!GetPlayerCharacter())
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UUIManager* UIManager = LocalPlayer ? LocalPlayer->GetSubsystem<UUIManager>() : nullptr;
	if (UIManager)
	{
		if (const AProjectPlayerCharacter* CurrentCharacter = GetPlayerCharacter();
			IsValid(CurrentCharacter) && !CurrentCharacter->bIsDead)
		{
			UIManager->PopWidget(FProjectGameplayTag::Get().UI_Widget_MainAttack_Death);
		}
	}
	UMainAttackUIWidgetController* WidgetController = UIManager
		? UIManager->GetMainAttackUIWidgetController(this)
		: nullptr;

	if (WidgetController)
	{
		WidgetController->OnChangeWidgetControllerParamSignature.Broadcast();
	}
}

void AProjectPlayerController::AddAbilityTagToPreInput(const FGameplayTag AbilityTag)
{
	if (PreInputTags.Contains(AbilityTag))
	{
		PreInputTags.Remove(AbilityTag);
	}
	PreInputTags.Add(AbilityTag);
}

bool AProjectPlayerController::FindActivatableAbilityTag()
{
	if (PreInputTags.IsEmpty()) return false;

	// 获取当前角色的 Category
	EPlayerCategory CurrentCategory = GetPlayerCharacter()->PlayerCategory;
	UInputInfo* InputInfo = UProjectBlueprintFunctionLibrary::GetConfigManager(this)->GetInputInfo();
	if (!InputInfo) return false;
	FInputSetting* InputSetting = InputInfo->PlayerCategoryToInputSetting.Find(CurrentCategory);
	if (!InputSetting) return false;


	// 倒序遍历（最新的输入优先）
	for (int32 i = PreInputTags.Num() - 1; i >= 0; --i)
	{
		const FGameplayTag& AbilityTag = PreInputTags[i];
		FGameplayAbilitySpec Spec;
		if (UProjectBlueprintFunctionLibrary::CanActivateAbilityAndOutputSpec(GetAbilitySystemComponent(), AbilityTag, Spec))
		{
			GetAbilitySystemComponent()->TryActivateAbility(Spec.Handle);
			return true;
		}
	}
	return false;
}


UProjectAbilitySystemComponent* AProjectPlayerController::GetAbilitySystemComponent()
{
	AProjectPlayerCharacter* CurrentCharacter = GetPlayerCharacter();
	UProjectAbilitySystemComponent* CurrentAbilitySystemComponent = IsValid(CurrentCharacter)
		? CurrentCharacter->GetAbilitySystemComponent()
		: nullptr;
	if (AbilitySystemComponent != CurrentAbilitySystemComponent)
	{
		AbilitySystemComponent = CurrentAbilitySystemComponent;
	}
	return AbilitySystemComponent;
}

UProjectGameInstance* AProjectPlayerController::GetProjectGameInstance()
{
	if (GameInstance==nullptr)
	{
		GameInstance=Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(this));
	}
	return GameInstance;
}

//使用的时候可以存储一个临时变量
AProjectPlayerCharacter* AProjectPlayerController::GetPlayerCharacter()
{
	PlayerCharacter=Cast<AProjectPlayerCharacter>(GetCharacter());
	return PlayerCharacter;
}

AProjectAttackHUD* AProjectPlayerController::GetProjectHUD()
{
	if (ProjectHUD==nullptr)
	{
		ProjectHUD=Cast<AProjectAttackHUD>(GetHUD());
	}
	return ProjectHUD;
}

void AProjectPlayerController::MoveActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Move)) return;

	FVector2D InputAxisVector=Value.Get<FVector2D>();
	if (InputAxisVector.Length()>1.f)
	{
		InputAxisVector.Normalize();
	}
	const FRotator ControllerRotation=GetControlRotation();
	const FRotator YawRotation=FRotator(0.f,ControllerRotation.Yaw,0.f);
	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControllerPawn=GetPawn())
	{
		ControllerPawn->AddMovementInput(ForwardDirection,InputAxisVector.X);
		ControllerPawn->AddMovementInput(RightDirection,InputAxisVector.Y);
	}
}

void AProjectPlayerController::LookActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Look)) return;
	if (TargetLockComponent && TargetLockComponent->IsLocking()) return;

	const FVector2D InputAxisVector=Value.Get<FVector2D>();
	const UProjectGameInstance* TempProjectGameInstance=GetProjectGameInstance();
	const float DeltaYaw=InputAxisVector.X*TempProjectGameInstance->MouseAxisXSensitivityScale;
	const float DeltaPitch=InputAxisVector.Y*TempProjectGameInstance->MouseAxisYSensitivityScale;
	AddPitchInput(DeltaPitch);
	AddYawInput(DeltaYaw);
}

void AProjectPlayerController::WheelActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	const float WheelValue = Value.Get<float>();
	if (FMath::IsNearlyZero(WheelValue))
	{
		return;
	}

	AProjectPlayerCharacter* ControlledCharacter = GetPlayerCharacter();
	if (!IsValid(ControlledCharacter))
	{
		return;
	}

	// 有当前交互目标时，由玩家交互组件切换目标，不同时缩放镜头。
	UPlayerInteractionComponent* InteractionComponent = ControlledCharacter->GetPlayerInteractionComponent();
	if (IsValid(InteractionComponent) && InteractionComponent->GetCurrentTarget())
	{
		InteractionComponent->CycleTarget(WheelValue > 0.0f ? 1 : -1);
		return;
	}

	if (GetAbilitySystemComponent() &&
		GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_SpringArm))
	{
		return;
	}

	if (!IsValid(ControlledCharacter->SpringArmComponent))
	{
		return;
	}
	UProjectGameInstance* ProjectGameInstance = GetProjectGameInstance();
	if (!IsValid(ProjectGameInstance))
	{
		return;
	}

	const float TargetArmLength = FMath::Clamp(
		ControlledCharacter->SpringArmComponent->TargetArmLength + WheelValue * ProjectGameInstance->SpringArmChangeSensitivityScale,
		ControlledCharacter->MinSpringArmLength,
		ControlledCharacter->MaxSpringArmLength);
	ControlledCharacter->SpringArmComponent->TargetArmLength = TargetArmLength;
}

void AProjectPlayerController::JumpActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Jump)) return;
	GetPlayerCharacter()->Jump();
}


void AProjectPlayerController::LockOnActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	if (TargetLockComponent) TargetLockComponent->ToggleLockOn();
}


void AProjectPlayerController::LockSwitchActionFunction(const FInputActionValue& Value)
{
	if (IsGameplayInputBlocked()) return;
	if (TargetLockComponent) TargetLockComponent->AddTargetSwitchInput(Value.Get<float>());
}


void AProjectPlayerController::OnAbilityInputStarted(const FGameplayTag AbilityTag)
{
	if (IsGameplayInputBlocked()) return;
	if (UProjectAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Ability))
		{
			return;
		}
		ASC->AbilityInputTagPressed(AbilityTag);
	}
}

void AProjectPlayerController::OnAbilityInputReleased(const FGameplayTag AbilityTag)
{
	GetAbilitySystemComponent()->AbilityInputTagReleased(AbilityTag);
}

void AProjectPlayerController::OnAbilityInputTriggered(const FGameplayTag AbilityTag)
{
	if (IsGameplayInputBlocked()) return;
	if (UProjectAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Ability))
		{
			return;
		}
		ASC->AbilityInputTagHeld(AbilityTag);
	}
}

void AProjectPlayerController::OnAbilityJudgeStarted(const FGameplayTag JudgeTag)
{
	if (IsGameplayInputBlocked()) return;
	if (UProjectAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		if (ASC->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Ability))
		{
			return;
		}
	}
	else
	{
		return;
	}

	if (bCanPreInput)
	{
		AddAbilityTagToPreInput(JudgeTag);
	}
	GetAbilitySystemComponent()->AbilityJudgeTagPressed(JudgeTag);
}

bool AProjectPlayerController::IsGameplayInputBlocked() const
{
	const AProjectPlayerCharacter* ControlledCharacter = Cast<AProjectPlayerCharacter>(GetPawn());
	return !IsValid(ControlledCharacter) || ControlledCharacter->bIsDead;
}

void AProjectPlayerController::RequestRestartGame()
{
	if (HasAuthority())
	{
		Server_RequestRestartGame_Implementation();
	}
	else
	{
		Server_RequestRestartGame();
	}
}

void AProjectPlayerController::Server_RequestRestartGame_Implementation()
{
	const AProjectPlayerCharacter* DeadCharacter = Cast<AProjectPlayerCharacter>(GetPawn());
	if (!IsValid(DeadCharacter) || !DeadCharacter->bIsDead)
	{
		return;
	}

	if (APlayGameModeBase* PlayGameMode = GetWorld()->GetAuthGameMode<APlayGameModeBase>())
	{
		PlayGameMode->RespawnDeadPlayer(this);
	}
}

void AProjectPlayerController::OnAbilityJudgeReleased(const FGameplayTag JudgeTag)
{

	GetAbilitySystemComponent()->AbilityInputTagReleased(JudgeTag);
}


void AProjectPlayerController::SwitchCharacterIMC(EPlayerCategory NewCategory)
{
	UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this);
	if (!InputManager) return;

	UInputInfo* InputInfo = UProjectBlueprintFunctionLibrary::GetConfigManager(this)->GetInputInfo();
	if (!InputInfo) return;

	FInputSetting* NewSetting = InputInfo->PlayerCategoryToInputSetting.Find(NewCategory);
	if (!NewSetting) return;

	if (CurrentSpecialIMCTag.IsValid())
	{
		InputManager->DeactivateMappedInputMappingContextByTag(CurrentSpecialIMCTag);
		CurrentSpecialIMCTag = FGameplayTag();
	}

	if (!NewSetting->SpecialIMCTag.IsValid()) return;

	UInputMappingContext* TempInputMappingContext = nullptr;
	int32 InputMappingPriority = 0;
	if (!InputInfo->GetIMCWithPriority(NewSetting->SpecialIMCTag, TempInputMappingContext, InputMappingPriority)) return;
	if (!InputManager->ActivateMappedInputMappingContextByTag(NewSetting->SpecialIMCTag, InputMappingPriority)) return;

	CurrentSpecialIMCTag = NewSetting->SpecialIMCTag;
}

void AProjectPlayerController::Server_SwitchCharacter_Implementation(const int32 TargetIndex)
{
	AProjectPlayerState* ProjectPlayerState = GetPlayerState<AProjectPlayerState>();
	if (!PlayerState || !ProjectPlayerState->TeamArray.IsValidIndex(TargetIndex)) return;

	AProjectPlayerCharacter* OldChar = Cast<AProjectPlayerCharacter>(GetPawn());
	AProjectPlayerCharacter* NewChar = ProjectPlayerState->TeamArray[TargetIndex].Character;

	if (!OldChar || !NewChar || OldChar == NewChar) return;

	const FVector Location = OldChar->GetActorLocation();
	const FRotator Rotation = OldChar->GetActorRotation();
	NewChar->SetActorLocationAndRotation(Location, Rotation);

	OldChar->SetActorHiddenInGame(true);
	OldChar->SetActorTickEnabled(false);
	OldChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OldChar->GetCharacterMovement()->Deactivate();

	NewChar->SetActorHiddenInGame(false);
	NewChar->SetActorTickEnabled(true);
	NewChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	NewChar->GetCharacterMovement()->Activate();

	this->Possess(NewChar);

	NewChar->GetAbilitySystemComponent()->InitAbilityActorInfo(NewChar, NewChar);
}
