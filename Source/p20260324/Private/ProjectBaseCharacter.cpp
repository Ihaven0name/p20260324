#include "ProjectBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "Net/UnrealNetwork.h"

AProjectBaseCharacter::AProjectBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AProjectBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

void AProjectBaseCharacter::AddInputWindowTimer_Implementation(const float InRate, const FGameplayTagContainer& InTags)
{
	if (InputWindowTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InputWindowTimerHandle);
		RemoveInputWindowTags();
	}
	InputWindowTagContainer=InTags;
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(this,InputWindowTagContainer);
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
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(this,InputWindowTagContainer);
}

UProjectAbilitySystemComponent* AProjectBaseCharacter::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

UProjectAttributeSet* AProjectBaseCharacter::GetAttributeSet()
{
	return AttributeSet;
}
