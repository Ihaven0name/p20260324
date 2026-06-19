#pragma once

#include "CoreMinimal.h"
#include "SingleControlWidgetController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "MainAttackUIWidgetController.generated.h"

struct FInputActionValue;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, float, NewValue);

UCLASS(Blueprintable)
class P20260324_API UMainAttackUIWidgetController : public USingleControlWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallback() override;
	UFUNCTION(BlueprintCallable, Category = "Character")
	void RequestCharacterSwitch(int32 TargetIndex) const;
	
};
