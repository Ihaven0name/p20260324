#include "UI/WidgetController/AttributeWidgetController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/ProjectAbilitySystemComponent.h"
#include "GAS/ProjectBlueprintFunctionLibrary.h"
#include "GAS/ProjectGameplayTag.h"
#include "Info/AttributeInfo.h"
#include "Manager/ConfigManager.h"
#include "Manager/InventoryManager.h"
#include "Manager/UIManager.h"
#include "p20260324/LogChannel.h"
#include "Player/ProjectPlayerController.h"

void UAttributeWidgetController::BindCallback()
{
	for (TTuple<FGameplayTag,FGameplayAttribute> &TempPair:FProjectGameplayTag::TagToAttributeMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TempPair.Value).AddLambda([TempPair, this](const FOnAttributeChangeData& InAttributeData)
		{
			FProjectAttributeInfo AuroAttributeInfo;
			AuroAttributeInfo.AttributeTag=TempPair.Key;
			AuroAttributeInfo.AttributeValue=TempPair.Value.GetNumericValue(AttributeSet);
			OnAttributeChangeSignature.Broadcast(AuroAttributeInfo);
		});
	}

	OnChangeWidgetControllerParamSignature.AddDynamic(this,&ThisClass::OnChangeWidgetControllerParamFunction);
	OnSelectXPElementsSignature.AddDynamic(this,&ThisClass::OnSelectXPElementsFunction);
	OnAcceptXPElementsSignature.AddDynamic(this,&ThisClass::OnAcceptXPElementsFunction);
}

void UAttributeWidgetController::BroadcastInitialAttribute() const
{
	for (TTuple<FGameplayTag,FGameplayAttribute> &TempPair:FProjectGameplayTag::TagToAttributeMap)
	{
		FProjectAttributeInfo AuroAttributeInfo;
		AuroAttributeInfo.AttributeTag=TempPair.Key;
		AuroAttributeInfo.AttributeValue=TempPair.Value.GetNumericValue(AttributeSet);
		OnAttributeChangeSignature.Broadcast(AuroAttributeInfo);
	}
}

void UAttributeWidgetController::BindingInputActionByTag(const FGameplayTag InputActionTag)
{
	UE_LOG(LogProject, Warning, TEXT("BindingInputActionByTag In MainAttackUIWidgetController"));
	const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();
	if (InputActionTag.MatchesTagExact(ProjectGameplayTag.UI_Action_Attribute_RemoveAttributeUI))
	{
		CloseAttributeUIActionFunction(ProjectGameplayTag.UI_Widget_Attribute);
	}
}

void UAttributeWidgetController::CloseAttributeUIActionFunction(const FGameplayTag WidgetTag)
{
	if (UUIManager* UIManager=UProjectBlueprintFunctionLibrary::GetUIManager(PlayerController))
	{
		UIManager->PopWidget(WidgetTag);
	}
}

void UAttributeWidgetController::OnAcceptXPElementsFunction(const int32 SmallElement, const int32 MediumElement, const int32 LargeElement)
{
	if (UInventoryManager* InventoryManager=UProjectBlueprintFunctionLibrary::GetInventoryManager(PlayerController))
	{
		const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();
		if (LargeElement==0 && MediumElement==0 && SmallElement==0) return;
		if (SmallElement!=0)
			InventoryManager->RemoveItem(ProjectGameplayTag.Inventory_Material_Experience_Small,SmallElement);
		if (MediumElement!=0)
			InventoryManager->RemoveItem(ProjectGameplayTag.Inventory_Material_Experience_Medium,MediumElement);
		if (LargeElement!=0)
			InventoryManager->RemoveItem(ProjectGameplayTag.Inventory_Material_Experience_Large,LargeElement);
		UProjectGameInstance* ProjectGameInstance=PlayerController->GetProjectGameInstance();
		const UConfigManager* ConfigManager= ProjectGameInstance->GetSubsystem<UConfigManager>();
		const int32 TempXP=
			SmallElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Small]+
			MediumElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Medium]+
			LargeElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Large];
		const UAttributeInfo* AttributeInfo=ConfigManager->GetAttributeInfo();
		UProjectAbilitySystemComponent* ProjectAbilitySystemComponent=PlayerCharacter->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle=ProjectAbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(PlayerCharacter);
		FGameplayEffectSpecHandle EffectSpecHandle=ProjectAbilitySystemComponent->MakeOutgoingSpec(AttributeInfo->AddIncomingXPEffectClass,1.f,EffectContextHandle);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,ProjectGameplayTag.Attribute_Meta_IncomingXP,TempXP);
		ProjectAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
	else
	{
		UE_LOG(LogProject, Warning, TEXT("OnAcceptXPElementsFunction Not Found InventoryManager"));
	}
}

void UAttributeWidgetController::OnSelectXPElementsFunction(const int32 SmallElement,const int32 MediumElement,const int32 LargeElement,bool& bCanApproachBound)
{
	const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();
	if (SmallElement==0 && MediumElement==0 && LargeElement==0)
	{
		const FProjectAttributeInfo LevelAttributeInfo=FProjectAttributeInfo(ProjectGameplayTag.Attribute_Primary_Level,AttributeSet->GetLevel());
		const FProjectAttributeInfo XPAttributeInfo=FProjectAttributeInfo(ProjectGameplayTag.Attribute_Primary_XP,AttributeSet->GetXP()); 
		OnAttributeChangeSignature.Broadcast(LevelAttributeInfo);
		OnAttributeChangeSignature.Broadcast(XPAttributeInfo);
		bCanApproachBound=false;
		return;
	}
	UProjectGameInstance* ProjectGameInstance=PlayerController->GetProjectGameInstance();
	const UConfigManager* ConfigManager= ProjectGameInstance->GetSubsystem<UConfigManager>();
	const int32 TempXP=
		SmallElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Small]+
		MediumElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Medium]+
		LargeElement*ProjectGameInstance->XPTagToXPValue[ProjectGameplayTag.Inventory_Material_Experience_Large];
	int CalculateLevel=0;
	int CalculateXP=0;
	UProjectBlueprintFunctionLibrary::CalculateCurrentLevel(AttributeSet,TempXP,CalculateLevel,CalculateXP);
	//可以到达经验上限
	if (CalculateLevel==ConfigManager->PlayerLevelBound)
	{
		bCanApproachBound=true;
	}
	const FProjectAttributeInfo LevelAttributeInfo=FProjectAttributeInfo(ProjectGameplayTag.Attribute_Primary_Level,CalculateLevel);
	OnAttributeChangeSignature.Broadcast(LevelAttributeInfo);
	OnTemporaryLevelAndXPChangeSignature.Broadcast(CalculateLevel,CalculateXP);
	bCanApproachBound=false;
} 