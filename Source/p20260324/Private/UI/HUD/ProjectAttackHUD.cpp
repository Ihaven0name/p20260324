

#include "UI/HUD/ProjectAttackHUD.h"

#include "UI/WidgetController/MainAttackUIWidgetController.h"
#include "UI/WidgetController/SettingUIWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/ProjectBaseWidgetController.h"
#include "UI/Widget/ProjectUserWidget.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UIManager.h"

void AProjectAttackHUD::BeginPlay()
{
	Super::BeginPlay();
	UUIManager* UIManager= UProjectBlueprintFunctionLibrary::GetUIManager(GetOwningPlayerController());
	UIManager->AddMainUI();
	UIManager->PushWidget(FProjectGameplayTag::Get().UI_Widget_MainAttack);
}