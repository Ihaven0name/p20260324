

#include "Player/ProjectPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilitySpec.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/ConfigManager.h"
#include "Player/ProjectPlayerController.h"
#include "Player/ProjectPlayerState.h"
#include "UI/HUD/ProjectAttackHUD.h"

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
	
}

void AProjectPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetOwner(NewController);
	InitAbilityActorInfo();
}

void AProjectPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	SetOwner(GetController());
	InitAbilityActorInfo();
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
	return GetProjectPlayerController()->bIsLocking;
}

AActor* AProjectPlayerCharacter::GetLockingActor_Implementation()
{
	return GetProjectPlayerController()->LockingActor;
}

EPlayerCategory AProjectPlayerCharacter::GetPlayerCategory_Implementation()
{
	return PlayerCategory;
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

void AProjectPlayerCharacter::CalculateCurrentLevel(const int32 TempXP, int& OutLevel, int& OutXP)
{
	UConfigManager* ConfigManager= UProjectBlueprintFunctionLibrary::GetConfigManager(this);
	OutLevel=GetAttributeSet()->GetLevel();
	OutXP=GetAttributeSet()->GetXP()+TempXP;
	while (OutLevel<ConfigManager->PlayerLevelBound)
	{
		const int NeedXP=ConfigManager->GetNeedXPAtLevel(OutLevel);
		if (OutXP>=NeedXP)
		{
			OutXP-=NeedXP;
			OutLevel+=1;
		}
		else
		{
			break;
		}
	}
	if (OutLevel==ConfigManager->PlayerLevelBound)
	{
		OutXP=0;
	}
}



