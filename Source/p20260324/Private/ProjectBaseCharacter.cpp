#include "ProjectBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AProjectBaseCharacter::AProjectBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 复制属性由服务器计算；AnimBP 在线程安全更新中只读取这些值。
	if (HasAuthority())
	{
		OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::UpdateMovementStateFromCMC);
		UpdateMovementStateFromCMC(0.0f, GetActorLocation(), GetVelocity());
	}
}

void AProjectBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectBaseCharacter, Level);
	DOREPLIFETIME(AProjectBaseCharacter, XP);
	DOREPLIFETIME(AProjectBaseCharacter, HasVelocity);
	DOREPLIFETIME(AProjectBaseCharacter, HasAcceleration);
	DOREPLIFETIME(AProjectBaseCharacter, bIsDead);
}

void AProjectBaseCharacter::SetIsDead_Implementation()
{
	if (!HasAuthority() || bIsDead)
	{
		return;
	}

	bIsDead = true;
	OnDeathStateChanged();
	OnCharacterDied.Broadcast(this);
	ForceNetUpdate();
}

void AProjectBaseCharacter::OnRep_IsDead()
{
	if (bIsDead)
	{
		OnDeathStateChanged();
	}
}

void AProjectBaseCharacter::OnDeathStateChanged()
{
}

void AProjectBaseCharacter::UpdateMovementStateFromCMC(const float DeltaSeconds, const FVector OldLocation, const FVector OldVelocity)
{
	if (!HasAuthority())
	{
		return;
	}

	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent)
	{
		return;
	}

	// 使用一个很小的阈值屏蔽浮点噪声，避免状态在零附近反复切换。
	constexpr float MovementThreshold = 1.0f;
	const float ThresholdSquared = FMath::Square(MovementThreshold);
	const bool bNewHasVelocity = MovementComponent->Velocity.SizeSquared() > ThresholdSquared;
	const bool bNewHasAcceleration = MovementComponent->GetCurrentAcceleration().SizeSquared() > ThresholdSquared;

	if (HasVelocity != bNewHasVelocity || HasAcceleration != bNewHasAcceleration)
	{
		HasVelocity = bNewHasVelocity;
		HasAcceleration = bNewHasAcceleration;
		ForceNetUpdate();
	}
}

// ============ IPlayerInterface ============

int32 AProjectBaseCharacter::GetLevel_Implementation()
{
	return Level;
}

void AProjectBaseCharacter::AddLevel_Implementation(const int32 InLevel)
{
	const int32 OldLevel = Level;
	Level += InLevel;
	if (Level < 0)
	{
		Level = 0;
	}
	OnRep_Level(OldLevel);
}

int32 AProjectBaseCharacter::GetXP_Implementation()
{
	return XP;
}

void AProjectBaseCharacter::AddXP_Implementation(const int32 InXP)
{
	const int32 OldXP = XP;
	XP += InXP;
	if (XP < 0)
	{
		XP = 0;
	}
	OnRep_XP(OldXP);
}

UProjectAbilitySystemComponent* AProjectBaseCharacter::GetAbilitySystemComponentByInterface_Implementation()
{
	return GetAbilitySystemComponent();
	
}

UAnimMontage* AProjectBaseCharacter::GetHitReactMontage_Implementation(const EHitReactDirection Direction)
{
	switch (Direction)
	{
	case EHitReactDirection::Front:
		return FrontHitReactMontage;
	case EHitReactDirection::Back:
		return BackHitReactMontage;
	case EHitReactDirection::Left:
		return LeftHitReactMontage;
	case EHitReactDirection::Right:
		return RightHitReactMontage;
	default:
		return nullptr;
	}
}

UAnimMontage* AProjectBaseCharacter::GetDeathMontage_Implementation()
{
	return DeathMontage;
}

void AProjectBaseCharacter::AddInputWindowTimer_Implementation(const float InRate, const FGameplayTagContainer& InTags)
{
	if (InputWindowTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InputWindowTimerHandle);
		RemoveInputWindowTags();
	}
	InputWindowTagContainer=InTags;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this, InputWindowTagContainer, HasAuthority());
	GetWorld()->GetTimerManager().SetTimer(
		InputWindowTimerHandle,
		this,
		&ThisClass::RemoveInputWindowTags,
		InRate
	);
}


// ============ OnRep ============

void AProjectBaseCharacter::OnRep_Level(const int32 OldLevel)
{
	// 可在此处添加升级逻辑
}

void AProjectBaseCharacter::OnRep_XP(const int32 OldXP)
{
	// 可在此处添加经验变化逻辑（如检查是否升级）
}

void AProjectBaseCharacter::RemoveInputWindowTags()
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this, InputWindowTagContainer, HasAuthority());
}

UProjectAbilitySystemComponent* AProjectBaseCharacter::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

UProjectAttributeSet* AProjectBaseCharacter::GetAttributeSet()
{
	return AttributeSet;
}
