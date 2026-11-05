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
#include "Engine/CurveTable.h"
#include "Interface/PlayerInterface.h"
#include "Info/CharacterInfo.h"

void UAttributeWidgetController::BindCallback()
{
	Super::BindCallback();
	for (TTuple<FGameplayTag,FGameplayAttribute> &TempPair:FProjectGameplayTag::TagToAttributeMap)
	{
		WidgetControllerParam.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TempPair.Value).AddLambda([TempPair, this](const FOnAttributeChangeData& InAttributeData)
		{
			FProjectAttributeInfo AuroAttributeInfo;
			AuroAttributeInfo.AttributeTag=TempPair.Key;
			AuroAttributeInfo.AttributeValue=TempPair.Value.GetNumericValue(WidgetControllerParam.AttributeSet);
			OnAttributeChangeSignature.Broadcast(AuroAttributeInfo);
		});
	}

	// SingleControlWidgetController already binds OnChangeWidgetControllerParamFunction.
	OnSelectXPElementsSignature.AddUniqueDynamic(this,&ThisClass::OnSelectXPElementsFunction);
	OnAcceptXPElementsSignature.AddUniqueDynamic(this,&ThisClass::OnAcceptXPElementsFunction);
}

void UAttributeWidgetController::BroadcastInitialAttribute() const
{
	for (TTuple<FGameplayTag,FGameplayAttribute> &TempPair:FProjectGameplayTag::TagToAttributeMap)
	{
		FProjectAttributeInfo AuroAttributeInfo;
		AuroAttributeInfo.AttributeTag=TempPair.Key;
		AuroAttributeInfo.AttributeValue=TempPair.Value.GetNumericValue(WidgetControllerParam.AttributeSet);
		OnAttributeChangeSignature.Broadcast(AuroAttributeInfo);
	}
}



void UAttributeWidgetController::OnAcceptXPElementsFunction(const int32 SmallElement, const int32 MediumElement, const int32 LargeElement)
{
	if (SmallElement < 0 || MediumElement < 0 || LargeElement < 0 ||
		(SmallElement == 0 && MediumElement == 0 && LargeElement == 0) || !PlayerController)
	{
		return;
	}

	PlayerController->RequestUpgradeWithExperienceMaterials(SmallElement, MediumElement, LargeElement);
}

void UAttributeWidgetController::OnSelectXPElementsFunction(const int32 SmallElement,const int32 MediumElement,const int32 LargeElement,bool& bCanApproachBound)
{
	const FProjectGameplayTag& ProjectGameplayTag=FProjectGameplayTag::Get();
	if (SmallElement==0 && MediumElement==0 && LargeElement==0)
	{
		BroadcastInitialAttribute();
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
	UProjectBlueprintFunctionLibrary::CalculateCurrentLevel(WidgetControllerParam.AttributeSet,TempXP,CalculateLevel,CalculateXP);
	BroadcastInitialAttribute();
	//可以到达经验上限
	if (CalculateLevel==ConfigManager->PlayerLevelBound)
	{
		bCanApproachBound=true;
	}
	const FProjectAttributeInfo LevelAttributeInfo=FProjectAttributeInfo(ProjectGameplayTag.Attribute_Primary_Level,CalculateLevel);
	OnAttributeChangeSignature.Broadcast(LevelAttributeInfo);

	// Preview level-scaled values from the same category curves used by the player MMCs.
	if (WidgetControllerParam.PlayerCharacter && WidgetControllerParam.PlayerCharacter->Implements<UPlayerInterface>())
	{
		const EPlayerCategory Category = IPlayerInterface::Execute_GetPlayerCategory(WidgetControllerParam.PlayerCharacter);
		const FString CategoryName = StaticEnum<EPlayerCategory>()->GetNameStringByValue(static_cast<int64>(Category));
		if (UCurveTable* CurveTable = ConfigManager->GetPlayerCurveTableByRowName(FName(*CategoryName)))
		{
			const TPair<FGameplayTag, FName> LevelScaledAttributes[] =
			{
				{ProjectGameplayTag.Attribute_Primary_MaxHealth, TEXT("MaxHealth")},
				{ProjectGameplayTag.Attribute_Primary_MaxMana, TEXT("MaxMana")},
				{ProjectGameplayTag.Attribute_Secondary_AttackPower, TEXT("AttackPower")},
				{ProjectGameplayTag.Attribute_Secondary_Defense, TEXT("Defense")}
			};

			for (const TPair<FGameplayTag, FName>& Attribute : LevelScaledAttributes)
			{
				if (const FRealCurve* Curve = CurveTable->FindCurve(Attribute.Value, TEXT("Attribute level preview")))
				{
					OnAttributeChangeSignature.Broadcast(FProjectAttributeInfo(Attribute.Key, Curve->Eval(CalculateLevel)));
				}
			}
		}
	}

	OnTemporaryLevelAndXPChangeSignature.Broadcast(CalculateLevel,CalculateXP);
	bCanApproachBound=false;
}

void UAttributeWidgetController::RequestChangeSwitcherIndex(const int32 NewIndex)
{
	OnSwitcherIndexChangedSignature.Broadcast(NewIndex);
}
