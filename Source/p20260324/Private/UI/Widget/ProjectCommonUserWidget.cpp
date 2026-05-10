#include "UI/Widget/ProjectCommonUserWidget.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Input/UIActionBinding.h"
#include "p20260324/LogChannel.h"
#include "UI/WidgetController/ProjectWidgetController.h"


class UCommonUIActionRouterBase;

UProjectCommonUserWidget::UProjectCommonUserWidget()
{
	bAutoActivate=false;
}

void UProjectCommonUserWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	AfterNativeOnActivated();
}

void UProjectCommonUserWidget::NativeOnDeactivated()
{
	RemoveAllActionBindings();
	bFinishBinding=false;
	BeforeNativeOnDeactivated();
	Super::NativeOnDeactivated();
}

void UProjectCommonUserWidget::SetWidgetController(UObject* Controller)
{
	UE_LOG(LogTemp, Warning, TEXT("[ProjectUserWidget] SetWidgetController: Widget=%s, Controller=%s"),
		*GetName(), Controller ? *Controller->GetClass()->GetName() : TEXT("nullptr"));
	WidgetController = Controller;
	AfterSetWidgetController();
	UE_LOG(LogTemp, Warning, TEXT("Widget Controller Set"));
}

UObject* UProjectCommonUserWidget::GetWidgetController()
{
	return WidgetController;
}

void UProjectCommonUserWidget::BindUIInputActionByTag(UProjectWidgetController* ProjectWidgetController)
{
	if (!bFinishBinding)
	{
		for (const auto& [ActionTag, InputAction] : ActionTagAndInputActions)
		{
			FSimpleDelegate OnExecuteAction;
			const FUIActionTag UIActionTag=FUIActionTag::ConvertChecked(ActionTag);
			OnExecuteAction.BindUObject(ProjectWidgetController,&UProjectWidgetController::BindingInputActionByTag,ActionTag);
		
			FBindUIActionArgs ActionArgs(InputAction,OnExecuteAction);
			ActionArgs.InputMode=CommonInputMode;
			ActionArgs.ActionTag=UIActionTag;
			RegisterUIActionBinding(ActionArgs);
		}
		bFinishBinding=true;
	}
}
void UProjectCommonUserWidget::RemoveAllActionBindings()
{
	TArray<FUIActionBindingHandle> TempActionBindings=GetActionBindings();
	for (FUIActionBindingHandle& TempHandle:TempActionBindings)
	{
		RemoveActionBinding(TempHandle);
		TempHandle.Unregister();
	}
}
