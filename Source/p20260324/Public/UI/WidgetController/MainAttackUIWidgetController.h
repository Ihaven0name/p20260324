#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectWidgetController.h"
#include "MainAttackUIWidgetController.generated.h"

struct FInputActionValue;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, float, NewValue);

UCLASS(Blueprintable)
class P20260324_API UMainAttackUIWidgetController : public UProjectWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallback() override;
	virtual void BindingInputActionByTag(const FGameplayTag InputActionTag) override;
	
	void OpenAttributeUIActionFunction() const;
	void OpenSettingUIActionFunction() const;
};
