// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/ConfigManager.h"
#include "Engine/Engine.h"

void UConfigManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 加载 CurveTable DataTable
	if (!BasedCurveDataTable)
	{
		BasedCurveDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/DataTable/DT_Based_Curve.DT_Based_Curve"));
	}

	//加载玩家经验曲线
	if (!PlayerLevelCurveFloat)
	{
		PlayerLevelCurveFloat = LoadObject<UCurveFloat>(nullptr,TEXT("/Game/Blueprint/CurveTable/CF_PlayerLevelCurveTable.CF_PlayerLevelCurveTable"));
	}
	

	// 加载 CharacterInfo
	if (!CharacterInfo)
	{
		CharacterInfo = LoadObject<UCharacterInfo>(nullptr, TEXT("/Game/Blueprint/DataAsset/DA_CharacterInfo.DA_CharacterInfo"));
	}

	// 加载 InputInfo
	if (!InputInfo)
	{
		InputInfo = LoadObject<UInputInfo>(nullptr, TEXT("/Game/Blueprint/DataAsset/DA_InputInfo.DA_InputInfo"));
	}
	
	// 加载 AttributeInfo
	if (!AttributeInfo)
	{
		AttributeInfo = LoadObject<UAttributeInfo>(nullptr,TEXT("/Game/Blueprint/DataAsset/DA_AttributeInfo.DA_AttributeInfo"));
	}

	if (!AbilityInfo)
	{
		AbilityInfo=LoadObject<UAbilityInfo>(nullptr,TEXT("/Game/Blueprint/DataAsset/DA_AbilityInfo.DA_AbilityInfo"));
	}

	if (!WidgetInfo)
	{
		WidgetInfo=LoadObject<UWidgetInfo>(nullptr,TEXT("/Game/Blueprint/DataAsset/DA_WidgetInfo.DA_WidgetInfo"));
	}
	
}

UCurveTable* UConfigManager::GetCurveTableByRowName(FName RowName) const
{
	if (!BasedCurveDataTable) return nullptr;

	const FMMCDataTable* Row = BasedCurveDataTable->FindRow<FMMCDataTable>(RowName, nullptr);
	
	return Row ? Row->CurveTable : nullptr;
}

float UConfigManager::GetNeedXPAtLevel(const int32 InLevel)
{
	if (InLevel>=PlayerLevelBound) return 0.f;
	return PlayerLevelCurveFloat->GetFloatValue(InLevel);
}