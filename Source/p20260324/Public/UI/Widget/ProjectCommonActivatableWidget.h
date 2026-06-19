#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Info/InputActionInfo.h"
#include "ProjectCommonActivatableWidget.generated.h"

class UProjectBaseWidgetController;
DECLARE_DYNAMIC_DELEGATE(FOnUIActionExecuted);
UCLASS()
class P20260324_API UProjectCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UProjectCommonActivatableWidget();
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Tag")
	TObjectPtr<UInputActionInfo> InputActionInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Tag")
	FGameplayTag StackLayerTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Input")
	bool bUseIMCTag = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Input", meta=(EditCondition="bUseIMCTag"))
	FGameplayTag IMCTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Config|Tag")
	ECommonInputMode CommonInputMode;
	

	//现在的逻辑是在UIManager里面进行绑定WidgetController
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);
	UFUNCTION(BlueprintCallable)
	UObject* GetWidgetController();
	
	UFUNCTION(BlueprintCallable)
	void UnbindActions();
	UFUNCTION(BlueprintCallable)
	void UnregisterActions();
	UFUNCTION(BlueprintImplementableEvent)
	void AfterSetWidgetController();
	UFUNCTION(BlueprintImplementableEvent)
	void AfterNativeOnActivated();
	UFUNCTION(BlueprintImplementableEvent)
	void BeforeNativeOnDeactivated();
	UFUNCTION(BlueprintImplementableEvent)
	void RegisterAction();

protected:
	UFUNCTION(BlueprintCallable)
	void BindAction(FGameplayTag ActionTag, const UInputAction* InputAction, FOnUIActionExecuted OnUIActionExecuted);

	
private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
	TMap<FGameplayTag, FUIActionBindingHandle> ActionBindingHandles;
	UPROPERTY()
	TMap<FGameplayTag, FOnUIActionExecuted> ActionDelegates;
	UFUNCTION()
	void ExecuteActionByTag(FGameplayTag ActionTag);
	
};
