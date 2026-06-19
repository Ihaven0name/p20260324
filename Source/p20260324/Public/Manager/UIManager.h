#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Player/ProjectPlayerController.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widget/ProjectCommonActivatableWidget.h"
#include "UI/Widget/ProjectUserWidget.h"
#include "UI/WidgetController/InventoryUIWidgetController.h"
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
	TObjectPtr<UProjectCommonActivatableWidget> UserWidget;
};


UCLASS()
class P20260324_API UUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddMainUI();
	
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UMainAttackUIWidgetController* GetMainAttackUIWidgetController(AProjectPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	USettingUIWidgetController* GetSettingUIWidgetController(AProjectPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UInventoryUIWidgetController* GetInventoryUIWidgetController(AProjectPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UAttributeWidgetController* GetAttributeWidgetController(AProjectPlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "UIManager|WidgetController")
	UProjectBaseWidgetController* GetWidgetControllerByWidgetTag(const FGameplayTag WidgetTag, AProjectPlayerController* PlayerController);

	
	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	UCommonActivatableWidgetStack* GetCommonStackByStackLayerTag(const FGameplayTag StackLayerTag);
	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	UCommonActivatableWidget* PushWidget(const FGameplayTag WidgetTag);
	UFUNCTION(BlueprintCallable, Category = "UIManager|Stack")
	void PopWidget(const FGameplayTag WidgetTag);
	UFUNCTION(BlueprintCallable)
	AProjectPlayerController* GetPlayerController() const;

	template <typename T>
	T* GetOrCreateWidgetController(const FGameplayTag WidgetTag, AProjectPlayerController* PlayerController);
	
private:
	UPROPERTY()
	TWeakObjectPtr<UProjectMainUIUserWidget> MainUIWidget;
	UPROPERTY()
	TMap<FGameplayTag,UProjectBaseWidgetController*> ControllerTagToWidgetController;
};
