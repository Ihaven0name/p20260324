#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/ProjectWidgetController.h"
#include "AttributeWidgetController.generated.h"


class UConfigManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, const FProjectAttributeInfo&,AttributeInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSelectXPElementsSignature,const int32,LargeElement,const int32,MiddleElement,const int32,SmallElement,bool&,bCanApproachBound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAcceptXPElementsSignature,const int32,SmallElement,const int32,MiddleElement,const int32,LargeElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTemporaryLevelAndXPChangeSignature,const int32,TempLevel,const int32,TempXP);




UCLASS(Blueprintable)
class P20260324_API UAttributeWidgetController : public UProjectWidgetController
{
	GENERATED_BODY()
public:
	
	virtual void BindCallback() override;
	virtual void BroadcastInitialAttribute() const override;
	virtual void BindingInputActionByTag(const FGameplayTag InputActionTag) override;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnAttributeChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSelectXPElementsSignature OnSelectXPElementsSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAcceptXPElementsSignature OnAcceptXPElementsSignature;
	UPROPERTY(BlueprintAssignable)
	FOnTemporaryLevelAndXPChangeSignature OnTemporaryLevelAndXPChangeSignature;
	UFUNCTION()
	void CloseAttributeUIActionFunction(const FGameplayTag WidgetTag);
	UFUNCTION()
	void OnAcceptXPElementsFunction(const int32 SmallElement,const int32 MediumElement,const int32 LargeElement);
	UFUNCTION()
	void OnSelectXPElementsFunction(const int32 LargeElement,const int32 MediumElement,const int32 SmallElement,bool& bCanApproachBound);
};
