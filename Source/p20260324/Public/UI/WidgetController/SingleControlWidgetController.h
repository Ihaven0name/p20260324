#pragma once

#include "CoreMinimal.h"
#include "Player/ProjectPlayerController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "SingleControlWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeWidgetControllerParamSignature);

UCLASS()
class P20260324_API USingleControlWidgetController : public UProjectBaseWidgetController
{
	GENERATED_BODY()
public:
	virtual void InitWidgetController(AProjectPlayerController* InPlayerController) override;
	virtual void BindCallback() override;
	
	
	UPROPERTY(BlueprintAssignable)
	FOnChangeWidgetControllerParamSignature OnChangeWidgetControllerParamSignature;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnChangeWidgetControllerParamFunction();
	
	UPROPERTY(BlueprintReadWrite)
	FWidgetControllerParam WidgetControllerParam;


protected:
	void RefreshWidgetControllerParam();
};
