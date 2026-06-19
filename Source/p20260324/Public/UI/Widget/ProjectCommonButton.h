#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GameplayTagContainer.h"
#include "ProjectCommonButton.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnButtonClickedDelegate);

class UProjectBaseWidgetController;

UCLASS()
class P20260324_API UProjectCommonButton : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	virtual void NativeDestruct() override;
	virtual void NativeOnActionComplete() override;
	virtual void NativeOnClicked() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Input")
	FGameplayTag ActionTag;
	UPROPERTY(BlueprintReadOnly)
	FOnButtonClickedDelegate OnClickedDelegate;
	UFUNCTION(BlueprintCallable)
	void SetOnClickedCallback(FOnButtonClickedDelegate Callback);
	UFUNCTION(BlueprintCallable)
	void ExecuteAction();
	
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);
	UFUNCTION(BlueprintCallable)
	UObject* GetWidgetController() const;
	UFUNCTION(BlueprintImplementableEvent)
	void AfterSetWidgetController();



private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
};
