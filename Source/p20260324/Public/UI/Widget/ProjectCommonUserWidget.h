#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ProjectCommonUserWidget.generated.h"

UCLASS()
class P20260324_API UProjectCommonUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);
	UFUNCTION(BlueprintCallable)
	UObject* GetWidgetController();
	UFUNCTION(BlueprintImplementableEvent)
	void AfterSetWidgetController();
private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
};
