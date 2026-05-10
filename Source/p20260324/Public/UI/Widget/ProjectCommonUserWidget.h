#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Info/InputInfo.h"
#include "ProjectCommonUserWidget.generated.h"

class UProjectWidgetController;

UCLASS()
class P20260324_API UProjectCommonUserWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UProjectCommonUserWidget();
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Tag")
	TArray<FActionInfo> ActionTagAndInputActions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Tag")
	FGameplayTag StackLayerTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|InputConfig")
	ECommonInputMode CommonInputMode;

	UPROPERTY()
	bool bFinishBinding=false;

	//现在的逻辑是在UIManager里面进行绑定WidgetController
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);
	UFUNCTION(BlueprintCallable)
	UObject* GetWidgetController();
	UFUNCTION()
	void BindUIInputActionByTag(UProjectWidgetController* ProjectWidgetController);
	UFUNCTION()
	void RemoveAllActionBindings();
	UFUNCTION(BlueprintImplementableEvent)
	void AfterSetWidgetController();
	//主要进行绑定SetWidgetController的事情，因为原来是在生成的时候进行绑定，现在在每次激活的时候进行绑定
	UFUNCTION(BlueprintImplementableEvent)
	void AfterNativeOnActivated();
	UFUNCTION(BlueprintImplementableEvent)
	void BeforeNativeOnDeactivated();

private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;

	
};
