#include "Player/ProjectPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProjectGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "Info/InputInfo.h"
#include "Kismet/GameplayStatics.h"

class UEnhancedInputLocalPlayerSubsystem;

AProjectPlayerController::AProjectPlayerController()
{
}

void AProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//TODO：先进行加载默认的IMC，后面添加修改功能之后再说
	if(UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputSystem->AddMappingContext(GetProjectGameInstance()->InputInfo->DefaultProjectInputMappingContext,0);
	}
	
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
	//TODO:后面需要实现一些存档的功能，并且能够做到读取存档的输入IMC映射来进行后面的加载,目前先简单绑定一下
	UEnhancedInputComponent* EnhancedInputComponent=CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&ThisClass::MoveActionFunction);
	EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this,&ThisClass::LookActionFunction);
	EnhancedInputComponent->BindAction(SpringArmScaleAction,ETriggerEvent::Triggered,this,&ThisClass::SpringArmScaleActionFunction);
}

void AProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

UProjectAbilitySystemComponent* AProjectPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent==nullptr)
	{
		AbilitySystemComponent=Cast<UProjectAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
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

void AProjectPlayerController::MoveActionFunction(const FInputActionValue& Value)
{
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
	const FVector2D InputAxisVector=Value.Get<FVector2D>();
	const UProjectGameInstance* TempProjectGameInstance=GetProjectGameInstance();
	const float DeltaYaw=InputAxisVector.X*TempProjectGameInstance->MouseAxisXSensitivityScale;
	const float DeltaPitch=InputAxisVector.Y*TempProjectGameInstance->MouseAxisYSensitivityScale;
	AddPitchInput(DeltaPitch);
	AddYawInput(DeltaYaw);
}

void AProjectPlayerController::SpringArmScaleActionFunction(const FInputActionValue& Value)
{
	const float InputAxisFloat=Value.Get<float>();
	int TargetArmLength=GetPlayerCharacter()->SpringArmComponent->TargetArmLength+InputAxisFloat*GetProjectGameInstance()->SpringArmChangeSensitivityScale;
	TargetArmLength=FMathf::Clamp(TargetArmLength,GetPlayerCharacter()->MinSpringArmLength,GetPlayerCharacter()->MaxSpringArmLength);
	GetPlayerCharacter()->SpringArmComponent->TargetArmLength=TargetArmLength;
	UE_LOG(LogTemp,Warning,TEXT("Spring Arm Scale Action Function"));
}
