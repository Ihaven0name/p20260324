#include "UI/WidgetController/MainAttackUIWidgetController.h"

#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Manager/UIManager.h"
#include "p20260324/LogChannel.h"
#include "Player/ProjectPlayerController.h"

void UMainAttackUIWidgetController::BindCallback()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnHealthChangeSignature.Broadcast(Data.NewValue);
	});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UProjectAttributeSet::GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		OnManaChangeSignature.Broadcast(Data.NewValue);
	});
	
	OnChangeWidgetControllerParamSignature.AddDynamic(this,&ThisClass::OnChangeWidgetControllerParamFunction);
}

void UMainAttackUIWidgetController::BindingInputActionByTag(const FGameplayTag InputActionTag)
{
	UE_LOG(LogProject, Warning, TEXT("BindingInputActionByTag In MainAttackUIWidgetController"));
	const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();
	if (InputActionTag.MatchesTagExact(ProjectGameplayTag.UI_Action_MainAttack_AddAttributeUI))
	{
		OpenAttributeUIActionFunction();
	}
	else if (InputActionTag.MatchesTagExact(ProjectGameplayTag.UI_Action_MainAttack_AddSettingUI))
	{
		// OpenSettingUIActionFunction();
	}
}


void UMainAttackUIWidgetController::OpenAttributeUIActionFunction() const
{
	UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController)->PushWidget(FProjectGameplayTag::Get().UI_Widget_Attribute);
}

void UMainAttackUIWidgetController::OpenSettingUIActionFunction() const
{
	UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController)->PushWidget(FProjectGameplayTag::Get().UI_Widget_Setting);
}
