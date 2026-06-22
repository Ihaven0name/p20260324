#include "UI/Widget/ProjectCommonActivatableWidget.h"

#include "EnhancedInputComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Manager/InputManager.h"
#include "UnLuaEx.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Lua callback registered for tag: %s"), *ActionTag.ToString());
	}
}

void UProjectCommonActivatableWidget::BindAction(const FGameplayTag ActionTag,const UInputAction* InputAction, FOnUIActionExecuted OnUIActionExecuted)
{
	ActionDelegates.Add(ActionTag, OnUIActionExecuted);
	BindEnhancedInputAction(ActionTag, InputAction);
}

void UProjectCommonActivatableWidget::UnbindActions()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
	{
		for (TTuple<FGameplayTag, uint32>& TempPair : ActionBindingHandles)
		{
			EnhancedInputComponent->RemoveBindingByHandle(TempPair.Value);
		}
	}
	ActionBindingHandles.Empty();
}

void UProjectCommonActivatableWidget::UnregisterActions()
{
	UnbindActions();
	ActionBindingHandles.Empty();
	ActionDelegates.Empty();
}

bool UProjectCommonActivatableWidget::BindEnhancedInputAction(const FGameplayTag ActionTag, const UInputAction* InputAction)
{
	if (!InputAction) return false;

	if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
	{
		if (uint32* OldHandle = ActionBindingHandles.Find(ActionTag))
			EnhancedInputComponent->RemoveBindingByHandle(*OldHandle);

		FEnhancedInputActionEventBinding& Binding = EnhancedInputComponent->BindAction(
			InputAction,
			ETriggerEvent::Started,
			this,
			&UProjectCommonActivatableWidget::ExecuteActionByTag,
			ActionTag);

		ActionBindingHandles.Add(ActionTag, Binding.GetHandle());
		return true;
	}

	return false;
}

UEnhancedInputComponent* UProjectCommonActivatableWidget::GetEnhancedInputComponent() const
{
	APlayerController* PlayerController = nullptr;
	if (const UProjectBaseWidgetController* BaseWidgetController = Cast<UProjectBaseWidgetController>(WidgetController))
		PlayerController = BaseWidgetController->PlayerController;
	if (!PlayerController) return nullptr;

	return Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
}
