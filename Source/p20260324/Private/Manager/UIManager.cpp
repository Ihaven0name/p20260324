#include "Manager/UIManager.h"

#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/WidgetInfo.h"
#include "Manager/ConfigManager.h"
#include "p20260324/LogChannel.h"
#include "UI/Widget/ProjectMainUIUserWidget.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "UI/WidgetController/SettingUIWidgetController.h"

class UInventoryUIWidgetController;

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

UMainAttackUIWidgetController* UUIManager::GetMainAttackUIWidgetController(AProjectPlayerController* PlayerController)
{
	return GetOrCreateWidgetController<UMainAttackUIWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_MainAttack,
		PlayerController
	);
}

UAttributeWidgetController* UUIManager::GetAttributeWidgetController(AProjectPlayerController* PlayerController)
{
	return GetOrCreateWidgetController<UAttributeWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_Attribute,
		PlayerController
	);
}
USettingUIWidgetController* UUIManager::GetSettingUIWidgetController(AProjectPlayerController* PlayerController)
{
	return GetOrCreateWidgetController<USettingUIWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_Setting,
		PlayerController
	);
}

UInventoryUIWidgetController* UUIManager::GetInventoryUIWidgetController(AProjectPlayerController* PlayerController)
{
	return GetOrCreateWidgetController<UInventoryUIWidgetController>(
		FProjectGameplayTag::Get().UI_Widget_Inventory,
		PlayerController
	);
}

UProjectBaseWidgetController* UUIManager::GetWidgetControllerByWidgetTag(
	const FGameplayTag WidgetTag,
	AProjectPlayerController* PlayerController
)
{
	if (const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer()))
	{
		FWidgetProperty WidgetProperty;
		if (ConfigManager->GetWidgetInfo()->FindWidgetProperty(WidgetTag, WidgetProperty))
		{
			if (UProjectBaseWidgetController** FoundPtr = ControllerTagToWidgetController.Find(WidgetProperty.ControllerTag))
			{
				return *FoundPtr;
			}
			TSubclassOf<UProjectBaseWidgetController> TempWidgetControllerClass;
			if (ConfigManager->GetWidgetInfo()->FindWidgetControllerClass(WidgetProperty.ControllerTag, TempWidgetControllerClass))
			{
				UProjectBaseWidgetController* WidgetController = NewObject<UProjectBaseWidgetController>(this, TempWidgetControllerClass);
				WidgetController->InitWidgetController(PlayerController);
				WidgetController->BindCallback();
				ControllerTagToWidgetController.Add(WidgetProperty.ControllerTag, WidgetController);
				return WidgetController;
			}
		}
	}
	return nullptr;
}

template <typename T>
T* UUIManager::GetOrCreateWidgetController(const FGameplayTag WidgetTag, AProjectPlayerController* PlayerController)
{
	if (const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer()))
	{
		FWidgetProperty WidgetProperty;
		if (ConfigManager->GetWidgetInfo()->FindWidgetProperty(WidgetTag, WidgetProperty))
		{
			if (UProjectBaseWidgetController** FoundPtr = ControllerTagToWidgetController.Find(WidgetProperty.ControllerTag))
			{
				return Cast<T>(*FoundPtr);
			}
			TSubclassOf<UProjectBaseWidgetController> TempWidgetControllerClass;
			if (ConfigManager->GetWidgetInfo()->FindWidgetControllerClass(WidgetProperty.ControllerTag, TempWidgetControllerClass))
			{
				UProjectBaseWidgetController* NewBase = NewObject<UProjectBaseWidgetController>(this, TempWidgetControllerClass);
				NewBase->InitWidgetController(PlayerController);
				if (T* SpecificController = Cast<T>(NewBase))
				{
					ControllerTagToWidgetController.Add(WidgetTag, SpecificController);
					SpecificController->BindCallback();
					return SpecificController;
				}
			}
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
	const UWidgetInfo* WidgetInfo = ConfigManager->GetWidgetInfo();
	const FWidgetProperty* WidgetProperty = WidgetInfo->WidgetTagToWidgetProperty.Find(WidgetTag);
	if (!WidgetProperty) return nullptr;
	if (!IsValid(MainUIWidget.Get()))
	{
		UE_LOG(LogTemp,Warning,TEXT("In UIManager,MainUI Is Not Valid"));
		return nullptr;
	}
	UCommonActivatableWidgetStack* Stack= GetCommonStackByStackLayerTag(WidgetProperty->StackLayerTag);
	if (!Stack)  return nullptr;
	for (UCommonActivatableWidget* Widget : Stack->GetWidgetList())
	{
		if (Widget->GetClass()==WidgetProperty->WidgetClass)
		{
			Stack->RemoveWidget(*Widget);
			break;
		}
	}
	UCommonActivatableWidget* CommonActivatableWidget = Stack->AddWidget(WidgetProperty->WidgetClass);
	UProjectCommonActivatableWidget* ProjectWidget=Cast<UProjectCommonActivatableWidget>(CommonActivatableWidget);
	UProjectBaseWidgetController* WidgetController = UProjectBlueprintFunctionLibrary::GetWidgetControllerByWidgetTag(GetPlayerController(),WidgetTag);
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
		UProjectCommonActivatableWidget* CommonActivatableWidget=Cast<UProjectCommonActivatableWidget>(Stack->GetActiveWidget());
		CommonActivatableWidget->DeactivateWidget();
	}
	else
	{
		UE_LOG(LogProject, Warning, TEXT("In UIManager,Cannot find Widget In Specific Stack"));
	}
}


AProjectPlayerController* UUIManager::GetPlayerController() const
{
	if (const ULocalPlayer* TempLocalPlayer = GetLocalPlayer())
	{
		if (AProjectPlayerController* TempPlayerController = Cast<AProjectPlayerController>(TempLocalPlayer->GetPlayerController(GetWorld())))
		{
			return TempPlayerController;
		}
	}
	return nullptr;
}