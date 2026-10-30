// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/ConfigManager.h"
#include "Engine/Engine.h"

void UConfigManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 加载玩家属性曲线配置
	if (!PlayerBasedCurveDataTable)
	{
		PlayerBasedCurveDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/DataTable/DT_PlayerBased_Curve.DT_PlayerBased_Curve"));
	}

	// 加载敌人属性曲线配置
	if (!EnemyBasedCurveDataTable)
	{
		EnemyBasedCurveDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Blueprint/DataTable/DT_EnemyBased_Curve.DT_EnemyBased_Curve"));
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

	if (!InventoryInfo)
	{
		InventoryInfo=LoadObject<UInventoryInfo>(nullptr,TEXT("/Game/Blueprint/DataAsset/DA_InventoryInfo.DA_InventoryInfo"));
	}
	if (!MappableInfo)
	{
		MappableInfo = LoadObject<UMappableInfo>(nullptr, TEXT("/Game/Blueprint/DataAsset/DA_MappableInfo.DA_MappableInfo"));
	}
	if (!TeamInfo)
	{
		TeamInfo = LoadObject<UTeamInfo>(nullptr,TEXT("/Game/Blueprint/DataAsset/DA_TeamInfo.DA_TeamInfo"));
	}
}

UCurveTable* UConfigManager::GetPlayerCurveTableByRowName(FName RowName) const
{
	if (!PlayerBasedCurveDataTable) return nullptr;

	const FMMCDataTable* Row = PlayerBasedCurveDataTable->FindRow<FMMCDataTable>(RowName, nullptr);
	
	return Row ? Row->CurveTable : nullptr;
}
UCurveTable* UConfigManager::GetEnemyCurveTableByRowName(FName RowName) const
{
	if (!EnemyBasedCurveDataTable) return nullptr;

	const FMMCDataTable* Row = EnemyBasedCurveDataTable->FindRow<FMMCDataTable>(RowName, nullptr);
	return Row ? Row->CurveTable : nullptr;
}

float UConfigManager::GetNeedXPAtLevel(const int32 InLevel)
{
	if (InLevel>=PlayerLevelBound) return 0.f;
	return PlayerLevelCurveFloat->GetFloatValue(InLevel);
}
