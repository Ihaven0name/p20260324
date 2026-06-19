#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/ProjectCommonActivatableWidget.h"
#include "UI/Widget/ProjectMainUIUserWidget.h"
#include "ProjectAttackHUD.generated.h"

class USettingUIWidgetController;
class UMainAttackUIWidgetController;
class UAttributeWidgetController;
class UProjectUserWidget;
struct FWidgetControllerParam;

UCLASS(Blueprintable)
class P20260324_API AProjectAttackHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;	
};
