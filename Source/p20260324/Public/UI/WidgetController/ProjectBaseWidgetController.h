#pragma once

#include "CoreMinimal.h"
#include "LuaValue.h"
#include "ProjectGameInstance.h"
#include "Info/InputActionInfo.h"
#include "ProjectBaseWidgetController.generated.h"

class UInputAction;
class UProjectCommonActivatableWidget;
class UProjectAttributeSet;
class UProjectAbilitySystemComponent;
class AProjectPlayerCharacter;
class AProjectPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangeSignature,const float, NewValue);





USTRUCT(BlueprintType)
struct P20260324_API FWidgetControllerParam
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AProjectPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectAttributeSet> AttributeSet;
};

UCLASS(Blueprintable)
class P20260324_API UProjectBaseWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnHealthChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnManaChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnMaxHealthChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnMaxManaChangeSignature;
	


	
	
	
	virtual void InitWidgetController(AProjectPlayerController* InPlayerController);
	//子类在继承的时候需要进行super
	virtual void BindCallback();
	UFUNCTION()
	virtual void BroadcastInitialAttribute() const;
	
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AProjectPlayerController> PlayerController;
	
	UFUNCTION(BlueprintCallable)
	void CloseUIInputActionFunction(const FGameplayTag WidgetTag) const;
	UFUNCTION(BlueprintCallable)
	void OpenUIInputActionFunction(const FGameplayTag WidgetTag) const;

};
