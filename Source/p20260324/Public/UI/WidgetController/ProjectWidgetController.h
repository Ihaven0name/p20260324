#pragma once

#include "CoreMinimal.h"
#include "ProjectGameInstance.h"
#include "UObject/NoExportTypes.h"
#include "ProjectWidgetController.generated.h"

class UProjectCommonUserWidget;
class UProjectAttributeSet;
class UProjectAbilitySystemComponent;
class AProjectPlayerCharacter;
class AProjectPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangeSignature,const float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeWidgetControllerParamSignature);

USTRUCT(BlueprintType)
struct P20260324_API FWidgetControllerParam
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AProjectPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AProjectPlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectAttributeSet> AttributeSet;
};

UCLASS(Blueprintable)
class P20260324_API UProjectWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnHealthChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangeSignature OnManaChangeSignature;
	UPROPERTY(BlueprintAssignable)
	FOnChangeWidgetControllerParamSignature OnChangeWidgetControllerParamSignature;

	
	
	
	virtual void InitWidgetController(const FWidgetControllerParam& Params);
	virtual void BindCallback();
	UFUNCTION()
	virtual void BroadcastInitialAttribute() const;
	UFUNCTION()
	virtual void BindingInputActionByTag(const FGameplayTag InputActionTag);
	



	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AProjectPlayerCharacter> PlayerCharacter;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AProjectPlayerController> PlayerController;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UProjectAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UProjectAttributeSet> AttributeSet;
	UFUNCTION(BlueprintCallable)
	void OnChangeWidgetControllerParamFunction();
};
