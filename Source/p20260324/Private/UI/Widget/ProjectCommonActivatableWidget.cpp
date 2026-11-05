#include "UI/Widget/ProjectCommonActivatableWidget.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/InputManager.h"
#include "Input/CommonUIInputTypes.h"
#include "UnLuaEx.h"

UProjectCommonActivatableWidget::UProjectCommonActivatableWidget()
{
	bAutoActivate=false;
	bIsFocusable=true;
}

void UProjectCommonActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	if (bUseIMCTag)
	{
		if (UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this))
			InputManager->ActivateMappedInputMappingContextByTag(IMCTag, InputMappingPriority);
	}
	AfterNativeOnActivated();
	
}

void UProjectCommonActivatableWidget::NativeOnDeactivated()
{
	BeforeNativeOnDeactivated();
	if (bUseIMCTag)
	{
		if (UInputManager* InputManager = UProjectBlueprintFunctionLibrary::GetInputManager(this))
			InputManager->DeactivateMappedInputMappingContextByTag(IMCTag);
	}
	Super::NativeOnDeactivated();
}

void UProjectCommonActivatableWidget::NativeDestruct()
{
	//为了清除Lua的函数，防止循环引用
	UnregisterActions();
	Super::NativeDestruct();
	
}


void UProjectCommonActivatableWidget::SetWidgetController(UObject* Controller)
{
	UnregisterActions();
	WidgetController = Controller;
	AfterSetWidgetController();
}

UObject* UProjectCommonActivatableWidget::GetWidgetController()
{
	return WidgetController;
}


void UProjectCommonActivatableWidget::ExecuteActionByTag(const FGameplayTag ActionTag)
{
	if (ActionDelegates.Contains(ActionTag))
	{
		ActionDelegates[ActionTag].ExecuteIfBound();
	}
}

void UProjectCommonActivatableWidget::BindAction(const FGameplayTag ActionTag,const UInputAction* InputAction, FOnUIActionExecuted OnUIActionExecuted)
{
	ActionDelegates.Add(ActionTag, OnUIActionExecuted);
	if (!InputAction)
	{
		return;
	}

	if (FUIActionBindingHandle* ExistingHandle = ActionBindingHandles.Find(ActionTag))
	{
		RemoveActionBinding(*ExistingHandle);
		ExistingHandle->Unregister();
	}

	FBindUIActionArgs BindArgs(
		InputAction,
		false,
		FSimpleDelegate::CreateUObject(this, &ThisClass::ExecuteActionByTag, ActionTag));
	BindArgs.InputMode = CommonInputMode;
	BindArgs.bConsumeInput = true;

	const FUIActionBindingHandle BindingHandle = RegisterUIActionBinding(BindArgs);
	if (BindingHandle.IsValid())
	{
		ActionBindingHandles.Add(ActionTag, BindingHandle);
	}
}

void UProjectCommonActivatableWidget::UnbindActions()
{
	for (TTuple<FGameplayTag, FUIActionBindingHandle>& Binding : ActionBindingHandles)
	{
		RemoveActionBinding(Binding.Value);
		Binding.Value.Unregister();
	}
	ActionBindingHandles.Empty();
}

void UProjectCommonActivatableWidget::UnregisterActions()
{
	UnbindActions();
	ActionBindingHandles.Empty();
	ActionDelegates.Empty();
}
