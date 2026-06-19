#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "MultiControlWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeWidgetControllerParamsSignature);


UCLASS()
class P20260324_API UMultiControlWidgetController : public UProjectBaseWidgetController
{
	GENERATED_BODY()
public:
	virtual void InitWidgetController(AProjectPlayerController* InPlayerController) override;
	virtual void BindCallback() override;

	UPROPERTY(BlueprintAssignable)
	FOnChangeWidgetControllerParamsSignature OnChangeWidgetControllerParamsSignature; 

	
	UFUNCTION(BlueprintCallable)
	virtual void OnChangeWidgetControllerParamsFunction();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widgets")
	TArray<FWidgetControllerParam> WidgetControllerParams;
protected:
	void RefreshWidgetControllerParams();
};
