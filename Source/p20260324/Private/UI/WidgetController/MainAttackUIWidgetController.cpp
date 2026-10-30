#include "UI/WidgetController/MainAttackUIWidgetController.h"

#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Manager/TeamManager.h"
#include "Manager/UIManager.h"
#include "p20260324/LogChannel.h"
#include "Player/ProjectPlayerController.h"

void UMainAttackUIWidgetController::BindCallback()
{
	Super::BindCallback();
	BindAttributeDelegates();
}

void UMainAttackUIWidgetController::BindAttributeDelegates()
{
	UProjectAbilitySystemComponent* AbilitySystemComponent = WidgetControllerParam.AbilitySystemComponent;
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	BoundAbilitySystemComponent = AbilitySystemComponent;
	HealthDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChangeSignature.Broadcast(Data.NewValue);
	});
	ManaDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChangeSignature.Broadcast(Data.NewValue);
	});
	MaxHealthDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChangeSignature.Broadcast(Data.NewValue);
	});
	MaxManaDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChangeSignature.Broadcast(Data.NewValue);
	});
}

void UMainAttackUIWidgetController::UnbindAttributeDelegates()
{
	if (UProjectAbilitySystemComponent* AbilitySystemComponent = BoundAbilitySystemComponent.Get())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetHealthAttribute()).Remove(HealthDelegateHandle);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetManaAttribute()).Remove(ManaDelegateHandle);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxHealthAttribute()).Remove(MaxHealthDelegateHandle);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxManaAttribute()).Remove(MaxManaDelegateHandle);
	}

	BoundAbilitySystemComponent.Reset();
	HealthDelegateHandle.Reset();
	ManaDelegateHandle.Reset();
	MaxHealthDelegateHandle.Reset();
	MaxManaDelegateHandle.Reset();
}

void UMainAttackUIWidgetController::OnChangeWidgetControllerParamFunction()
{
	UnbindAttributeDelegates();
	Super::OnChangeWidgetControllerParamFunction();
	BindAttributeDelegates();
	BroadcastInitialAttribute();
}

void UMainAttackUIWidgetController::BroadcastInitialAttribute() const
{
	if (!WidgetControllerParam.AttributeSet)
	{
		return;
	}

	OnHealthChangeSignature.Broadcast(WidgetControllerParam.AttributeSet->GetHealth());
	OnManaChangeSignature.Broadcast(WidgetControllerParam.AttributeSet->GetMana());
	OnMaxHealthChangeSignature.Broadcast(WidgetControllerParam.AttributeSet->GetMaxHealth());
	OnMaxManaChangeSignature.Broadcast(WidgetControllerParam.AttributeSet->GetMaxMana());
}


void UMainAttackUIWidgetController::RequestCharacterSwitch(const int32 TargetIndex) const
{
	if (AProjectPlayerController* ProjectPC = Cast<AProjectPlayerController>(PlayerController))
	{
		ProjectPC->Server_SwitchCharacter(TargetIndex);
		const UTeamManager* TeamManager=UProjectBlueprintFunctionLibrary::GetTeamManager(ProjectPC);
		TeamManager->SetControlledCharacterIndex(TargetIndex);
	}
}
