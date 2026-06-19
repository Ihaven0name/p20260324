#include "UI/WidgetController/ProjectBaseWidgetController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CommonInputSettings.h"
#include "UnLuaEx.h"
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

void UProjectBaseWidgetController::InitWidgetController(AProjectPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

void UProjectBaseWidgetController::BindCallback()
{
}

void UProjectBaseWidgetController::BroadcastInitialAttribute() const
{
}


void UProjectBaseWidgetController::CloseUIInputActionFunction(const FGameplayTag WidgetTag) const
{
	if (UUIManager* UIManager=UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController))
	{
		UIManager->PopWidget(WidgetTag);
	}
}
void UProjectBaseWidgetController::OpenUIInputActionFunction(const FGameplayTag WidgetTag) const
{
	if (UUIManager* UIManager = UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController))
	{
		UIManager->PushWidget(WidgetTag);
	}
}
