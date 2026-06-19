#include "UI/Widget/ProjectCommonActivatableWidget.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/InputManager.h"
#include "UnLuaEx.h"
#include "Input/CommonUIInputTypes.h"
#include "Input/UIActionBinding.h"


class UCommonUIActionRouterBase;

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
	UnbindActions();
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
	UE_LOG(LogTemp, Warning, TEXT("[ProjectUserWidget] SetWidgetController: Widget=%s, Controller=%s"),
		*GetName(), Controller ? *Controller->GetClass()->GetName() : TEXT("nullptr"));
	WidgetController = Controller;
	AfterSetWidgetController();
	UE_LOG(LogTemp, Warning, TEXT("Widget Controller Set"));
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Lua callback registered for tag: %s"), *ActionTag.ToString());
	}
}

void UProjectCommonActivatableWidget::BindAction(const FGameplayTag ActionTag,const UInputAction* InputAction, FOnUIActionExecuted OnUIActionExecuted)
{
	if (const FUIActionBindingHandle* OldHandle = ActionBindingHandles.Find(ActionTag))
	{
		AddActionBinding(*OldHandle);
		return;
	}
	FSimpleDelegate OnExecuteAction;
	OnExecuteAction.BindUObject(this, &UProjectCommonActivatableWidget::ExecuteActionByTag, ActionTag);

	FBindUIActionArgs ActionArgs(InputAction, OnExecuteAction);
	ActionArgs.InputMode = CommonInputMode;
	ActionArgs.ActionTag = FUIActionTag::ConvertChecked(ActionTag);

	ActionBindingHandles.Add(ActionTag, RegisterUIActionBinding(ActionArgs));
	ActionDelegates.Add(ActionTag, OnUIActionExecuted);
}

void UProjectCommonActivatableWidget::UnbindActions()
{
	for (TTuple<FGameplayTag, FUIActionBindingHandle>& TempPair: ActionBindingHandles)
	{
		RemoveActionBinding(TempPair.Value);
	}
}

void UProjectCommonActivatableWidget::UnregisterActions()
{
	for (TTuple<FGameplayTag, FUIActionBindingHandle>& TempPair: ActionBindingHandles)
	{
		TempPair.Value.Unregister();
	}
	ActionBindingHandles.Empty();
}
