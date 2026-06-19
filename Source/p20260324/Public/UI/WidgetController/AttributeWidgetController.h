#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SingleControlWidgetController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "AttributeWidgetController.generated.h"


class UConfigManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, const FProjectAttributeInfo&,AttributeInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSelectXPElementsSignature,const int32,SmallElement,const int32,MiddleElement,const int32,LargeElement,bool&,bCanApproachBound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAcceptXPElementsSignature,const int32,SmallElement,const int32,MiddleElement,const int32,LargeElement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTemporaryLevelAndXPChangeSignature,const int32,TempLevel,const int32,TempXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitcherIndexChangedSignature,const int32,NewIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedNumberChangedSignature);





UCLASS(Blueprintable)
class P20260324_API UAttributeWidgetController : public USingleControlWidgetController
{
	GENERATED_BODY()
public:
	
	virtual void BindCallback() override;
	virtual void BroadcastInitialAttribute() const override;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnAttributeChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSelectXPElementsSignature OnSelectXPElementsSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAcceptXPElementsSignature OnAcceptXPElementsSignature;
	UPROPERTY(BlueprintAssignable)
	FOnTemporaryLevelAndXPChangeSignature OnTemporaryLevelAndXPChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSwitcherIndexChangedSignature OnSwitcherIndexChangedSignature;
	UPROPERTY(BlueprintAssignable)
	FOnSelectedNumberChangedSignature OnSelectedNumberChangedSignature;
	UFUNCTION()
	void OnAcceptXPElementsFunction(const int32 SmallElement,const int32 MediumElement,const int32 LargeElement);
	UFUNCTION()
	void OnSelectXPElementsFunction(const int32 SmallElement,const int32 MediumElement,const int32 LargeElement,bool& bCanApproachBound);
	UFUNCTION(BlueprintCallable)
	void RequestChangeSwitcherIndex(const int32 NewIndex);
};
