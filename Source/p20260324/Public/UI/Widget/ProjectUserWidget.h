#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectUserWidget.generated.h"

class UProjectBaseWidgetController;

UCLASS()
class P20260324_API UProjectUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float WidgetLength = 256.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float WidgetHeight = 256.f;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);

	UFUNCTION(BlueprintCallable)
	UObject* GetWidgetController();

	UFUNCTION(BlueprintImplementableEvent)
	void AfterSetWidgetController();

	
private:
	UPROPERTY()
	bool bAddToViewport=false;
};
