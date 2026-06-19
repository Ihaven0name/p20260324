#include "UI/Widget/ProjectCommonUserWidget.h"

void UProjectCommonUserWidget::SetWidgetController(UObject* Controller)
{
	WidgetController = Controller;
	AfterSetWidgetController();
}

UObject* UProjectCommonUserWidget::GetWidgetController()
{
	return WidgetController;
}
