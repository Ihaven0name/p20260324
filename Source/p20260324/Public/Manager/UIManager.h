#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widget/ProjectCommonUserWidget.h"
#include "UI/Widget/ProjectUserWidget.h"
#include "UIManager.generated.h"

class UCommonActivatableWidgetStack;
struct FWidgetControllerParam;
class UAttributeWidgetController;
class USettingUIWidgetController;
class UMainAttackUIWidgetController;
class UProjectMainUIUserWidget;

USTRUCT(BlueprintType)
struct FUITagAndUserWidget
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag UITag;
	UPROPERTY(BLueprintReadWrite)
	TObjectPtr<UProjectCommonUserWidget> UserWidget;
};


UCLASS()
class P20260324_API UUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddMainUI();
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UMainAttackUIWidgetController* GetMainAttackUIWidgetController(const FWidgetControllerParam& Params);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	USettingUIWidgetController* GetSettingUIWidgetController(const FWidgetControllerParam& Params);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParam& Params);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UProjectWidgetController* GetWidgetControllerByWidgetTag(const FWidgetControllerParam& Params,const FGameplayTag WidgetTag);

	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	UCommonActivatableWidgetStack* GetCommonStackByStackLayerTag(const FGameplayTag StackLayerTag);
	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	UCommonActivatableWidget* PushWidget(const FGameplayTag WidgetTag);
	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	void PopWidget(const FGameplayTag WidgetTag);

	
	template<typename T>
	T* GetOrCreateWidgetController(const FGameplayTag WidgetTag, const FWidgetControllerParam& Params);
	
private:
	UPROPERTY()
	TWeakObjectPtr<UProjectMainUIUserWidget> MainUIWidget;
	UPROPERTY()
	TMap<FGameplayTag,UProjectWidgetController*> UITagToWidgetController;
};
