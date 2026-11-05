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
	if (!ConfigManager)
	{
		return;
	}

	const UWidgetInfo* WidgetInfo = ConfigManager->GetWidgetInfo();
	if (!WidgetInfo)
	{
		return;
	}

	const TSubclassOf<UProjectMainUIUserWidget> WidgetClass = WidgetInfo->MainUIClass;
	if (!WidgetClass)
	{
		return;
	}

	MainUIWidget = CreateWidget<UProjectMainUIUserWidget>(GetWorld(), WidgetClass);
	if (!IsValid(MainUIWidget.Get()))
	{
		return;
	}

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
					ControllerTagToWidgetController.Add(WidgetProperty.ControllerTag, SpecificController);
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
	if (!WidgetTag.IsValid())
	{
		return nullptr;
	}

	const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer());
	if (!ConfigManager)
	{
		return nullptr;
	}

	const UWidgetInfo* WidgetInfo = ConfigManager->GetWidgetInfo();
	if (!WidgetInfo)
	{
		return nullptr;
	}

	const FWidgetProperty* WidgetProperty = WidgetInfo->WidgetTagToWidgetProperty.Find(WidgetTag);
	if (!WidgetProperty)
	{
		return nullptr;
	}

	if (!IsValid(MainUIWidget.Get()))
	{
		return nullptr;
	}

	UCommonActivatableWidgetStack* Stack = GetCommonStackByStackLayerTag(WidgetProperty->StackLayerTag);
	if (!Stack)
	{
		return nullptr;
	}
	if (!WidgetProperty->WidgetClass)
	{
		return nullptr;
	}

	for (UCommonActivatableWidget* Widget : Stack->GetWidgetList())
	{
		if (Widget->GetClass()==WidgetProperty->WidgetClass)
		{
			Stack->RemoveWidget(*Widget);
			break;
		}
	}

	UCommonActivatableWidget* CommonActivatableWidget = Stack->AddWidget(WidgetProperty->WidgetClass);
	if (!IsValid(CommonActivatableWidget))
	{
		return nullptr;
	}

	UProjectCommonActivatableWidget* ProjectWidget = Cast<UProjectCommonActivatableWidget>(CommonActivatableWidget);
	if (!ProjectWidget)
	{
		return nullptr;
	}

	UProjectBaseWidgetController* WidgetController = UProjectBlueprintFunctionLibrary::GetWidgetControllerByWidgetTag(GetPlayerController(), WidgetTag);
	if (!WidgetController)
	{
	}
	ProjectWidget->SetWidgetController(WidgetController);
	ProjectWidget->ActivateWidget();
	return ProjectWidget;
}

void UUIManager::PopWidget(const FGameplayTag WidgetTag)
{
	if (!WidgetTag.IsValid())
	{
		return;
	}

	const UConfigManager* ConfigManager = UProjectBlueprintFunctionLibrary::GetConfigManager(GetLocalPlayer());
	if (!ConfigManager || !ConfigManager->GetWidgetInfo())
	{
		return;
	}

	const FWidgetProperty* WidgetProperty = ConfigManager->GetWidgetInfo()->WidgetTagToWidgetProperty.Find(WidgetTag);
	if (!WidgetProperty)
	{
		return;
	}

	if (!IsValid(MainUIWidget.Get()))
	{
		return;
	}

	UCommonActivatableWidgetStack* Stack = GetCommonStackByStackLayerTag(WidgetProperty->StackLayerTag);
	if (!Stack)
	{
		return;
	}

	UCommonActivatableWidget* ActiveWidget = Stack->GetActiveWidget();
	if (!ActiveWidget)
	{
		return;
	}

	if (UProjectCommonActivatableWidget* ProjectWidget = Cast<UProjectCommonActivatableWidget>(ActiveWidget))
	{
		ProjectWidget->DeactivateWidget();
	}
	else
	{
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
