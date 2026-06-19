#include "UI/Widget/ProjectCommonButton.h"

#include "UITag.h"
#include "UnLuaBase.h"
#include "p20260324/LogChannel.h"

void UProjectCommonButton::NativeDestruct()
{
	OnClickedDelegate.Unbind(); 
	Super::NativeDestruct();
}

void UProjectCommonButton::NativeOnActionComplete()
{
	Super::NativeOnActionComplete();
	
}
void UProjectCommonButton::NativeOnClicked()
{
	Super::NativeOnClicked();
	ExecuteAction();
}

void UProjectCommonButton::SetOnClickedCallback(const FOnButtonClickedDelegate Callback)
{
	OnClickedDelegate = Callback;
	UE_LOG(LogUnLua,Warning,TEXT("%s"),*this->GetName())
}

void UProjectCommonButton::SetWidgetController(UObject* Controller)
{
	WidgetController=Controller;
	AfterSetWidgetController();
	
}

UObject* UProjectCommonButton::GetWidgetController() const
{
	return WidgetController;
}
void UProjectCommonButton::ExecuteAction()
{
	if (!OnClickedDelegate.ExecuteIfBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Lua callback registered for tag: %s"), *ActionTag.ToString());
	}
}