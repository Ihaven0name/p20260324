#include "UI/WidgetController/SingleControlWidgetController.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"

void USingleControlWidgetController::InitWidgetController(AProjectPlayerController* InPlayerController)
{
	Super::InitWidgetController(InPlayerController);
	RefreshWidgetControllerParam();
}

void USingleControlWidgetController::OnChangeWidgetControllerParamFunction()
{
	RefreshWidgetControllerParam();
}

void USingleControlWidgetController::BindCallback()
{
	OnChangeWidgetControllerParamSignature.AddDynamic(this,&ThisClass::OnChangeWidgetControllerParamFunction);
}

void USingleControlWidgetController::RefreshWidgetControllerParam()
{
	WidgetControllerParam.PlayerCharacter = PlayerController->GetPlayerCharacter();
	WidgetControllerParam.AttributeSet = WidgetControllerParam.PlayerCharacter->GetAttributeSet();
	WidgetControllerParam.AbilitySystemComponent = WidgetControllerParam.PlayerCharacter->GetAbilitySystemComponent();
}