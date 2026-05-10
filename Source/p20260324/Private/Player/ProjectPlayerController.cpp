#include "Player/ProjectPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "ProjectBaseCharacter.h"
#include "ProjectGameInstance.h"
#include "Manager/ConfigManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Info/InputInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/EnemyManager.h"
#include "Abilities/GameplayAbility.h"
#include "GAS/ProjectGameplayTag.h"
#include "Input/ProjectInputComponent.h"
#include "Manager/UIManager.h"
#include "UI/HUD/ProjectAttackHUD.h"
#include "UI/WidgetController/AttributeWidgetController.h"

class UEnhancedInputLocalPlayerSubsystem;

AProjectPlayerController::AProjectPlayerController()
{
	
}


void AProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//TODO：先进行加载默认的IMC，后面添加修改功能之后再说
	BindInputMappingContext();
	
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
	EnhancedInputComponent->BindAction(SpringArmScaleAction,ETriggerEvent::Triggered,this,&ThisClass::SpringArmScaleActionFunction);
	EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ThisClass::JumpActionFunction);
	EnhancedInputComponent->BindAction(LockOnAction,ETriggerEvent::Started,this,&ThisClass::LockOnActionFunction);
	EnhancedInputComponent->BindAction(LockSwitchAction,ETriggerEvent::Triggered,this,&ThisClass::LockSwitchActionFunction);
	// EnhancedInputComponent->BindAction(ChangeAttributeUIAction,ETriggerEvent::Started,this,&ThisClass::ChangeAttributeUIActionFunction);
	// EnhancedInputComponent->BindAction(ChangeSettingUIAction,ETriggerEvent::Started,this,&ThisClass::ChangeSettingUIActionFunction);
	//

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

void AProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	// 更新锁定时的旋转插值
	if (bIsLocking)
	{
		UpdateLockOnRotationInterpolation(DeltaTime);
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
	if (AbilitySystemComponent==nullptr)
	{
		AbilitySystemComponent=GetPlayerCharacter()->GetAbilitySystemComponent();
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

AProjectPlayerCharacter* AProjectPlayerController::GetPlayerCharacter()
{
	if (PlayerCharacter==nullptr)
	{
		PlayerCharacter=Cast<AProjectPlayerCharacter>(GetCharacter());
	}
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


void AProjectPlayerController::BindInputMappingContext()
{
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UInputInfo* InputInfo = UProjectBlueprintFunctionLibrary::GetConfigManager(this)->GetInputInfo();
		if (!InputInfo) return;

		EnhancedInputSystem->AddMappingContext(InputInfo->DefaultProjectAttackInputMappingContext,0);
		// 记录当前激活的 SpecialIMC
		FInputSetting* CurrentSetting = InputInfo->PlayerCategoryToInputSetting.Find(GetPlayerCharacter()->PlayerCategory);
		if (CurrentSetting && CurrentSetting->SpecialProjectInputMappingContext)
		{
			ActiveSpecialIMC = CurrentSetting->SpecialProjectInputMappingContext;
			EnhancedInputSystem->AddMappingContext(ActiveSpecialIMC, 1);
		}
	}
}


void AProjectPlayerController::MoveActionFunction(const FInputActionValue& Value)
{
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
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Look)) return;
	if (bIsLocking) return;

	const FVector2D InputAxisVector=Value.Get<FVector2D>();
	const UProjectGameInstance* TempProjectGameInstance=GetProjectGameInstance();
	const float DeltaYaw=InputAxisVector.X*TempProjectGameInstance->MouseAxisXSensitivityScale;
	const float DeltaPitch=InputAxisVector.Y*TempProjectGameInstance->MouseAxisYSensitivityScale;
	AddPitchInput(DeltaPitch);
	AddYawInput(DeltaYaw);
}

void AProjectPlayerController::SpringArmScaleActionFunction(const FInputActionValue& Value)
{
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_SpringArm)) return;

	const float InputAxisFloat=Value.Get<float>();
	int TargetArmLength=GetPlayerCharacter()->SpringArmComponent->TargetArmLength+InputAxisFloat*GetProjectGameInstance()->SpringArmChangeSensitivityScale;
	TargetArmLength=FMathf::Clamp(TargetArmLength,GetPlayerCharacter()->MinSpringArmLength,GetPlayerCharacter()->MaxSpringArmLength);
	GetPlayerCharacter()->SpringArmComponent->TargetArmLength=TargetArmLength;
}

void AProjectPlayerController::JumpActionFunction(const FInputActionValue& Value)
{
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(FProjectGameplayTag::Get().Block_InputAction_Jump)) return;
	GetPlayerCharacter()->Jump();
}


void AProjectPlayerController::LockOnActionFunction(const FInputActionValue& Value)
{
	if (bIsLocking)
	{
		bIsLocking=false;
		LockingActor=nullptr;
		GetPlayerCharacter()->bUseControllerRotationYaw=false;
		GetPlayerCharacter()->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		UEnemyManager* EnemyManager=UProjectBlueprintFunctionLibrary::GetEnemyManager(this);
		const TArray<AActor*>& ScreenActors=EnemyManager->GetVisibleEnemies(GetPlayerCharacter());
		AActor* TargetActor=UProjectBlueprintFunctionLibrary::FindNearestActor(GetPlayerCharacter(),ScreenActors);
		if (TargetActor==nullptr) return;
		
		bIsLocking=true;
		LockingActor=TargetActor;
		GetPlayerCharacter()->bUseControllerRotationYaw=true;
		GetPlayerCharacter()->GetCharacterMovement()->bOrientRotationToMovement = false;
		LerpCurrentRotation = GetControlRotation();
	}
}


void AProjectPlayerController::LockSwitchActionFunction(const FInputActionValue& Value)
{
	if (!bIsLocking) return;
	if (!LockingActor) return;
	const float InputAxisFloat=Value.Get<float>();
	SwitchTargetNumber+=InputAxisFloat * GetProjectGameInstance()->LockOnSwitchSensitivity;
	if (SwitchTargetNumber>SwitchTargetThreshold)
	{
		UEnemyManager* EnemyManager=UProjectBlueprintFunctionLibrary::GetEnemyManager(GetPlayerCharacter());
		if (AActor* NewActor = EnemyManager->GetSideEnemy(GetPlayerCharacter(),LockingActor,true,MaxLockAngle))
		{
			LockingActor=NewActor;
		}
		SwitchTargetNumber=0.f;
	}
	else if (SwitchTargetNumber<-SwitchTargetThreshold)
	{
		UEnemyManager* EnemyManager=UProjectBlueprintFunctionLibrary::GetEnemyManager(GetPlayerCharacter());
		if (AActor* NewActor = EnemyManager->GetSideEnemy(GetPlayerCharacter(),LockingActor,false,MaxLockAngle))
		{
			LockingActor=NewActor;
		}
		SwitchTargetNumber=0.f;
	}
}


void AProjectPlayerController::UpdateLockOnRotationInterpolation(float DeltaTime)
{
	const FVector TargetDirection = LockingActor->GetActorLocation() - GetPlayerCharacter()->GetActorLocation();
	FRotator TargetRotation = TargetDirection.Rotation();
	TargetRotation.Pitch=LockOnPitchRadius;
	TargetRotation.Normalize();
	// 使用插值逐渐转向目标方向
	LerpCurrentRotation = FMath::RInterpTo(LerpCurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
	// 设置控制器旋转
	SetControlRotation(LerpCurrentRotation);
}


void AProjectPlayerController::OnAbilityInputStarted(const FGameplayTag AbilityTag)
{
	GetAbilitySystemComponent()->AbilityInputTagPressed(AbilityTag);
}

void AProjectPlayerController::OnAbilityInputReleased(const FGameplayTag AbilityTag)
{
	GetAbilitySystemComponent()->AbilityInputTagReleased(AbilityTag);
}

void AProjectPlayerController::OnAbilityInputTriggered(const FGameplayTag AbilityTag)
{
	GetAbilitySystemComponent()->AbilityInputTagHeld(AbilityTag);
}

void AProjectPlayerController::OnAbilityJudgeStarted(const FGameplayTag JudgeTag)
{
	if (bCanPreInput)
	{
		AddAbilityTagToPreInput(JudgeTag);
	}
	GetAbilitySystemComponent()->AbilityJudgeTagPressed(JudgeTag);
}

void AProjectPlayerController::OnAbilityJudgeReleased(const FGameplayTag JudgeTag)
{

	GetAbilitySystemComponent()->AbilityInputTagReleased(JudgeTag);
}


void AProjectPlayerController::SwitchCharacterIMC(EPlayerCategory NewCategory)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// 1. 移除旧角色的 SpecialIMC
		if (ActiveSpecialIMC)
		{
			EnhancedInputSystem->RemoveMappingContext(ActiveSpecialIMC);
			ActiveSpecialIMC = nullptr;
		}

		// 2. 添加新角色的 SpecialIMC
		UInputInfo* InputInfo = UProjectBlueprintFunctionLibrary::GetConfigManager(this)->GetInputInfo();
		if (!InputInfo) return;
		FInputSetting* NewSetting = InputInfo->PlayerCategoryToInputSetting.Find(NewCategory);
		if (NewSetting && NewSetting->SpecialProjectInputMappingContext)
		{
			ActiveSpecialIMC = NewSetting->SpecialProjectInputMappingContext;
			EnhancedInputSystem->AddMappingContext(ActiveSpecialIMC, 1);
		}
	}
}