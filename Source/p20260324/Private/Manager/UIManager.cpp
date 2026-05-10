#include "Manager/UIManager.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/WidgetInfo.h"
#include "Manager/ConfigManager.h"
#include "p20260324/LogChannel.h"
#include "UI/Widget/ProjectMainUIUserWidget.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "UI/WidgetController/ProjectWidgetController.h"
#include "UI/WidgetController/SettingUIWidgetController.h"

void UUIManager::AddMainUI()
{
	const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer());
	TSubclassOf<UProjectMainUIUserWidget>& WidgetClass = ConfigManager->GetWidgetInfo()->MainUIClass;
	if (!WidgetClass)
	{
		UE_LOG(LogTemp,Warning,TEXT("In UIManager,MainUIClass Is Not Valid"));
	}
	MainUIWidget=CreateWidget<UProjectMainUIUserWidget>(GetWorld(), WidgetClass);
	MainUIWidget->AddToViewport();
}

template <typename T>
T* UUIManager::GetOrCreateWidgetController(const FGameplayTag WidgetTag, const FWidgetControllerParam& Params)
{
	if (UProjectWidgetController** FoundPtr = UITagToWidgetController.Find(WidgetTag))
	{
		return Cast<T>(*FoundPtr);
	}
	if (const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer()))
	{
		FWidgetProperty WidgetProperty;
		if (ConfigManager->GetWidgetInfo()->FindWidgetProperty(WidgetTag, WidgetProperty))
		{
			UProjectWidgetController* NewBase = NewObject<UProjectWidgetController>(this, WidgetProperty.WidgetControllerClass);
			if (T* SpecificController = Cast<T>(NewBase))
			{
				SpecificController->InitWidgetController(Params);
				UITagToWidgetController.Add(WidgetTag, SpecificController);
				return SpecificController;
			}
		}
	}
	return nullptr;
}
UMainAttackUIWidgetController* UUIManager::GetMainAttackUIWidgetController(const FWidgetControllerParam& Params)
{
	return GetOrCreateWidgetController<UMainAttackUIWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_MainAttack, 
		Params
	);
}

USettingUIWidgetController* UUIManager::GetSettingUIWidgetController(const FWidgetControllerParam& Params)
{
	return GetOrCreateWidgetController<USettingUIWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_Setting, 
		Params
	);
}

UAttributeWidgetController* UUIManager::GetAttributeWidgetController(const FWidgetControllerParam& Params)
{
	return GetOrCreateWidgetController<UAttributeWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_Attribute, 
		Params
	);
}

UProjectWidgetController* UUIManager::GetWidgetControllerByWidgetTag(
	const FWidgetControllerParam& Params,
	const FGameplayTag WidgetTag
)
{
	if (UProjectWidgetController** FoundPtr = UITagToWidgetController.Find(WidgetTag))
	{
		return *FoundPtr;
	}
	if (const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer()))
	{
		FWidgetProperty WidgetProperty;
		if (ConfigManager->GetWidgetInfo()->FindWidgetProperty(WidgetTag, WidgetProperty))
		{
			UProjectWidgetController* WidgetController = NewObject<UProjectWidgetController>(this, WidgetProperty.WidgetControllerClass);
			WidgetController->InitWidgetController(Params);
			WidgetController->BindCallback();
			UITagToWidgetController.Add(WidgetTag, WidgetController);
			return WidgetController;
		}
	}
	return nullptr;
}

UCommonActivatableWidgetStack* UUIManager::GetCommonStackByStackLayerTag(const FGameplayTag StackLayerTag)
{
	if (!StackLayerTag.IsValid()) return nullptr;
	const FProjectGameplayTag& Tags = FProjectGameplayTag::Get();
	if (StackLayerTag.MatchesTagExact(Tags.UI_Layer_Gameplay))
	{
		return MainUIWidget->Stack_Gameplay;
	}
	if (StackLayerTag.MatchesTagExact(Tags.UI_Layer_Prompt))
	{
		return MainUIWidget->Stack_Prompt;
	}
	UE_LOG(LogProject, Warning, TEXT("UIManager: Cannot find Stack for Tag: %s"), *StackLayerTag.ToString());
	return nullptr;
}

UCommonActivatableWidget* UUIManager::PushWidget(const FGameplayTag WidgetTag)
{
	if (!WidgetTag.IsValid()) return nullptr;
	const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer());
	const FWidgetProperty* WidgetProperty = ConfigManager->GetWidgetInfo()->WidgetTagToWidgetProperty.Find(WidgetTag);
	if (!WidgetProperty) return nullptr;
	if (!IsValid(MainUIWidget.Get()))
	{
		UE_LOG(LogTemp,Warning,TEXT("In UIManager,MainUI Is Not Valid"));
		return nullptr;
	}
	UCommonActivatableWidgetStack* Stack= GetCommonStackByStackLayerTag(WidgetProperty->StackLayerTag);
	if (!Stack)  return nullptr;
	const TArray<UCommonActivatableWidget*>& WidgetList= Stack->GetWidgetList();
	for (UCommonActivatableWidget* Widget : WidgetList)
	{
		if (Widget->GetClass()==WidgetProperty->WidgetClass)
		{
			Stack->RemoveWidget(*Widget);
			break;
		}
	}
	UCommonActivatableWidget* CommonActivatableWidget = Stack->AddWidget(WidgetProperty->WidgetClass);
	UProjectCommonUserWidget* ProjectWidget=Cast<UProjectCommonUserWidget>(CommonActivatableWidget);
	UProjectWidgetController* WidgetController = UProjectBlueprintFunctionLibrary::GetWidgetControllerByWidgetTag(CommonActivatableWidget,WidgetTag);
	ProjectWidget->SetWidgetController(WidgetController);
	ProjectWidget->ActivateWidget();
	UE_LOG(LogTemp, Warning, TEXT("Pushing Widget: %s"), *WidgetTag.ToString())
	return ProjectWidget;
}

void UUIManager::PopWidget(const FGameplayTag WidgetTag)
{
	if (!WidgetTag.IsValid()) return;
	const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer());
	const FWidgetProperty* WidgetProperty = ConfigManager->GetWidgetInfo()->WidgetTagToWidgetProperty.Find(WidgetTag);
	if (!WidgetProperty) return;
	if (!IsValid(MainUIWidget.Get()))
	{
		UE_LOG(LogTemp,Warning,TEXT("In UIManager,MainUI Is Not Valid"));
		return;
	}
	UCommonActivatableWidgetStack* Stack= GetCommonStackByStackLayerTag(WidgetProperty->StackLayerTag);
	if (Stack->GetActiveWidget())
	{
		UProjectCommonUserWidget* CommonActivatableWidget=Cast<UProjectCommonUserWidget>(Stack->GetActiveWidget());
		CommonActivatableWidget->DeactivateWidget();
	}
	else
	{
		UE_LOG(LogProject, Warning, TEXT("In UIManager,Cannot find Widget In Specific Stack"));
	}
}
