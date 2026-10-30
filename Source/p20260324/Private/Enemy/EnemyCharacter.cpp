#include "Enemy/EnemyCharacter.h"
#include "Animation/ProjectAnimInstance.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/EnemyManager.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/ProjectUserWidget.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widget/ProjectCommonUserWidget.h"

//TODO:小地图系统,准备做一个大世界

AEnemyCharacter::AEnemyCharacter()
{
	bReplicates = true;

	// ASC - Enemy 使用 Minimal 复制模式
	AbilitySystemComponent = CreateDefaultSubobject<UProjectAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UProjectAttributeSet>("AttributeSet");	
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());

	// 血条 WidgetComponent
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetCapsuleComponent());

}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemyCharacter, bIsChasing);
	DOREPLIFETIME(AEnemyCharacter, bIsCombating);
	DOREPLIFETIME(AEnemyCharacter, bIsIdling);
	DOREPLIFETIME(AEnemyCharacter, bIsTurnAnimationActive);
	DOREPLIFETIME(AEnemyCharacter, TurnAnimationRotationSpeed);
}

void AEnemyCharacter::SetTurnAnimationState(const bool bIsActive, const float SignedRotationSpeed)
{
	if (!HasAuthority())
	{
		return;
	}

	bIsTurnAnimationActive = bIsActive;
	TurnAnimationRotationSpeed = bIsActive ? SignedRotationSpeed : 0.0f;
	ApplyTurnAnimationStateToAnimInstance();
	ForceNetUpdate();
}

void AEnemyCharacter::OnRep_TurnAnimationState()
{
	ApplyTurnAnimationStateToAnimInstance();
}

void AEnemyCharacter::ApplyTurnAnimationStateToAnimInstance()
{
	USkeletalMeshComponent* MeshComponent = GetMesh();
	UProjectAnimInstance* AnimInstance = MeshComponent ? Cast<UProjectAnimInstance>(MeshComponent->GetAnimInstance()) : nullptr;
	if (!AnimInstance)
	{
		return;
	}

	if (bIsTurnAnimationActive)
	{
		AnimInstance->StartTurnToTarget(TurnAnimationRotationSpeed);
	}
	else
	{
		AnimInstance->StopTurnToTarget();
	}
}

void AEnemyCharacter::SetAIState(bool bNewIsChasing, bool bNewIsCombating, bool bNewIsIdling)
{
	if (!HasAuthority() || (bIsDead && (bNewIsChasing || bNewIsCombating || bNewIsIdling)))
	{
		return;
	}

	if (bIsChasing == bNewIsChasing && bIsCombating == bNewIsCombating && bIsIdling == bNewIsIdling)
	{
		return;
	}

	bIsChasing = bNewIsChasing;
	bIsCombating = bNewIsCombating;
	bIsIdling = bNewIsIdling;

	ForceNetUpdate();
}

void AEnemyCharacter::SetIsCombating(bool bNewIsCombating)
{
	if (!HasAuthority() || (bIsDead && bNewIsCombating) || bIsCombating == bNewIsCombating)
	{
		return;
	}

	bIsCombating = bNewIsCombating;
	ForceNetUpdate();
}

void AEnemyCharacter::SetTargetActor_Implementation(AActor* NewTargetActor)
{
	if (!HasAuthority())
	{
		return;
	}
	AProjectBaseCharacter* NewTargetCharacter = Cast<AProjectBaseCharacter>(NewTargetActor);
	if (bIsDead || (NewTargetCharacter && NewTargetCharacter->bIsDead))
	{
		NewTargetActor = nullptr;
	}

	if (ChasingActor != NewTargetActor)
	{
		UnbindTargetDeath();
		ChasingActor = NewTargetActor;

		if (NewTargetCharacter)
		{
			TargetDeathListener = NewTargetCharacter;
			TargetDeathDelegateHandle = NewTargetCharacter->OnCharacterDied.AddUObject(
				this, &ThisClass::HandleTargetDeath);
		}
	}

	if (AAIController* EnemyAIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* Blackboard = EnemyAIController->GetBlackboardComponent())
		{
			if (NewTargetActor)
			{
				Blackboard->SetValueAsObject(TEXT("TargetActor"), NewTargetActor);
			}
			else
			{
				Blackboard->ClearValue(TEXT("TargetActor"));
			}
		}
	}
}

void AEnemyCharacter::HandleTargetDeath(AActor* DeadActor)
{
	if (!HasAuthority() || ChasingActor != DeadActor)
	{
		return;
	}

	SetTargetActor_Implementation(nullptr);
	SetAIState(false, false, true);
}

void AEnemyCharacter::UnbindTargetDeath()
{
	if (AProjectBaseCharacter* TargetCharacter = TargetDeathListener.Get())
	{
		if (TargetDeathDelegateHandle.IsValid())
		{
			TargetCharacter->OnCharacterDied.Remove(TargetDeathDelegateHandle);
		}
	}

	TargetDeathListener.Reset();
	TargetDeathDelegateHandle.Reset();
}

AActor* AEnemyCharacter::GetTargetActor_Implementation() const
{
	return ChasingActor.Get();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	RegisterWithEnemyManager();

	// Enemy 只在服务器（权威端）初始化能力和属性
	if (HasAuthority())
	{
		UProjectBlueprintFunctionLibrary::InitializeEnemyStartupAbilities(AbilitySystemComponent.Get(), EnemyCategory);
		UProjectBlueprintFunctionLibrary::InitializeEnemyAttributeByEffect(AbilitySystemComponent.Get(), EnemyCategory, EnemyLevel);
	}

	BindHealthWidgetCallbacks();
	if (HealthWidgetComponent)
	{
		if (GetNetMode() != NM_DedicatedServer)
        {
			// Force widget creation before assigning its controller; clients can otherwise miss this one-shot setup.
			HealthWidgetComponent->InitWidget();
			if (UProjectCommonUserWidget* HealthWidget = Cast<UProjectCommonUserWidget>(HealthWidgetComponent->GetWidget()))
			{
				HealthWidget->SetWidgetController(this);
			}
        }
		HealthWidgetBaseDrawSize = HealthWidgetComponent->GetDrawSize();
		HealthWidgetComponent->SetHiddenInGame(true, true);
	}
}

void AEnemyCharacter::OnDeathStateChanged()
{
	Super::OnDeathStateChanged();
	if (HasAuthority())
	{
		SetAIState(false, false, false);
		SetTargetActor_Implementation(nullptr);

		if (AAIController* EnemyController = Cast<AAIController>(GetController()))
		{
			if (UBrainComponent* Brain = EnemyController->GetBrainComponent())
			{
				Brain->StopLogic(TEXT("Enemy died"));
			}
			EnemyController->StopMovement();
		}

		if (UProjectAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			ASC->CancelAllAbilities();
			ASC->SetUserAbilityActivationInhibited(true);
		}
		UnbindTargetDeath();

		if (UCharacterMovementComponent* Movement = GetCharacterMovement())
		{
			Movement->StopMovementImmediately();
		}

		SetLifeSpan(FMath::Max(0.0f, DeathLifeSpan));
	}
}

void AEnemyCharacter::RegisterWithEnemyManager()
{
	if (UEnemyManager* EnemyManager = UProjectBlueprintFunctionLibrary::GetEnemyManager(this))
	{
		EnemyManager->AddEnemy(this);
	}
}

void AEnemyCharacter::UpdateHealthBarByPlayerLocation_Implementation(const FVector PlayerLocation, const bool bIsVisible)
{
	if (!HealthWidgetComponent)
	{
		return;
	}

	if (!bIsVisible)
	{
		HealthWidgetComponent->SetHiddenInGame(true, true);
		return;
	}

	const float Distance = FVector::Distance(GetActorLocation(), PlayerLocation);
	const float HideDistance = FMath::Max(HealthBarMaxScaleDistance, HealthBarMinScaleDistance);
	const bool bShouldShow = Distance <= HideDistance;
	HealthWidgetComponent->SetHiddenInGame(!bShouldShow, true);
	if (!bShouldShow)
	{
		return;
	}

	const float DistanceRange = HealthBarMaxScaleDistance - HealthBarMinScaleDistance;
	const float Alpha = DistanceRange > UE_SMALL_NUMBER
		? FMath::Clamp((Distance - HealthBarMinScaleDistance) / DistanceRange, 0.0f, 1.0f)
		: 1.0f;
	// 距离越远比例越小：近处使用最大比例，远处逐渐缩到最小比例。
	const float Scale = FMath::Lerp(HealthBarMaxScale, HealthBarMinScale, Alpha);
	// 当前血条使用屏幕空间，显示像素尺寸由 DrawSize 控制。
	HealthWidgetComponent->SetDrawSize(HealthWidgetBaseDrawSize * Scale);
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Scale: %f"), Scale));
}

void AEnemyCharacter::BindHealthWidgetCallbacks()
{
	if (!AbilitySystemComponent) return;

	UProjectAttributeSet* ProjectAS = Cast<UProjectAttributeSet>(AttributeSet);
	if (!ProjectAS) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectAS->GetHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		float MaxHealth = AttributeSet->GetMaxHealth();
		float Percent = MaxHealth > 0.f ? Data.NewValue / MaxHealth : 0.f;
		OnEnemyHealthPercentChanged.Broadcast(Percent);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectAS->GetMaxHealthAttribute())
	.AddLambda([this](const FOnAttributeChangeData& Data)
	{
		float Health = AttributeSet->GetHealth();
		float Percent = Data.NewValue > 0.f ? Health / Data.NewValue : 0.f;
		OnEnemyHealthPercentChanged.Broadcast(Percent);
	});
}

