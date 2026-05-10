#include "UI/WidgetController/ProjectWidgetController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CommonInputSettings.h"
#include "VREditorMode.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectAttributeSet.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Manager/UIManager.h"
#include "p20260324/LogChannel.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectPlayerController.h"

void UProjectWidgetController::InitWidgetController(const FWidgetControllerParam& Params)
{
	PlayerCharacter = Params.PlayerCharacter;
	PlayerController = Params.PlayerController;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UProjectWidgetController::BindCallback()
{
	
}

void UProjectWidgetController::BroadcastInitialAttribute() const
{
	
}

void UProjectWidgetController::BindingInputActionByTag(const FGameplayTag InputActionTag)
{
	UE_LOG(LogProject, Warning, TEXT("BindingInputActionByTag In Base"));
}

void UProjectWidgetController::OnChangeWidgetControllerParamFunction()
{
	AProjectPlayerCharacter* ProjectCharacter = Cast<AProjectPlayerCharacter>(PlayerController->GetCharacter());
	UProjectAttributeSet* ProjectAttributeSet = Cast<UProjectAttributeSet>(ProjectCharacter->GetAttributeSet());
	UProjectAbilitySystemComponent* ASC = Cast<UProjectAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ProjectCharacter));

	this->PlayerCharacter = ProjectCharacter;
	//TODO:这啥情况
	this->PlayerController = PlayerController;
	this->AbilitySystemComponent = ASC;
	this->AttributeSet=ProjectAttributeSet;
}

