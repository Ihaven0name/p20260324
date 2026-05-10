#include "UI/Widget/ProjectUserWidget.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"

void UProjectUserWidget::SetWidgetController(UObject* Controller)
{
	UE_LOG(LogTemp, Warning, TEXT("[ProjectUserWidget] SetWidgetController: Widget=%s, Controller=%s"),
		*GetName(), Controller ? *Controller->GetClass()->GetName() : TEXT("nullptr"));
	WidgetController = Controller;
	AfterSetWidgetController();
	UE_LOG(LogTemp, Warning, TEXT("Widget Controller Set"));
}

UObject* UProjectUserWidget::GetWidgetController()
{
	return WidgetController;
}