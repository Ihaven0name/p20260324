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
	WidgetControllerParam.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChangeSignature.Broadcast(Data.NewValue);
	});
	WidgetControllerParam.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChangeSignature.Broadcast(Data.NewValue);
	});
	WidgetControllerParam.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChangeSignature.Broadcast(Data.NewValue);
	});
	WidgetControllerParam.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnMaxManaChangeSignature.Broadcast(Data.NewValue);
	});
	
	OnChangeWidgetControllerParamSignature.AddDynamic(this,&ThisClass::OnChangeWidgetControllerParamFunction);
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