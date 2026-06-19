// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Info/CharacterInfo.h"
#include "Info/InputInfo.h"
#include "Info/AbilityInfo.h"
#include "Info/AttributeInfo.h"
#include "Info/InventoryInfo.h"
#include "Info/MappableInfo.h"
#include "Info/WidgetInfo.h"
#include "Info/TeamInfo.h"
#include "ConfigManager.generated.h"


USTRUCT(BlueprintType)
struct FMMCDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveTable> CurveTable;
};

UCLASS()
class P20260324_API UConfigManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/** 初始化：加载 DataTable */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;




	UPROPERTY(BlueprintReadWrite)
	int32 PlayerLevelBound=10;

	
	/**
	 * 根据 RowName 获取对应的 CurveTable
	 * @param RowName - DT_Based_Curve 中的行名称（如 "Crouch"）
	 * @return 找到的 CurveTable，未找到返回 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Config")
	UCurveTable* GetCurveTableByRowName(FName RowName) const;


	UFUNCTION(BlueprintCallable, Category = "Config")
	float GetNeedXPAtLevel(const int32 InLevel);

	/** 获取 CharacterInfo 数据资产 */
	UFUNCTION(BlueprintCallable, Category = "Config")
	UCharacterInfo* GetCharacterInfo() const { return CharacterInfo; }

	/** 获取 InputInfo 输入配置数据资产 */
	UFUNCTION(BlueprintCallable, Category = "Config")
	UInputInfo* GetInputInfo() const { return InputInfo; }

	UFUNCTION(BlueprintCallable, Category = "Config")
	UAttributeInfo* GetAttributeInfo()const {return AttributeInfo;}

	UFUNCTION(BlueprintCallable, Category = "Config")
	UAbilityInfo* GetAbilityInfo()const {return AbilityInfo;}

	UFUNCTION(BlueprintCallable, Category = "Config")
	UWidgetInfo* GetWidgetInfo() const {return WidgetInfo;}

	UFUNCTION(BlueprintCallable, Category = "Config")
	UInventoryInfo* GetInventoryInfo()const {return InventoryInfo;}

	UFUNCTION(BlueprintCallable, Category = "Config")
	UMappableInfo* GetMappableInfo() const { return MappableInfo; }

	UFUNCTION(BlueprintCallable, Category = "Config")
	UTeamInfo* GetTeamInfo()const {return TeamInfo;}
	

private:
	/** 引用全局曲线配置 DataTable */
	UPROPERTY(EditAnywhere, Category = "Config|Curve")
	TObjectPtr<UDataTable> BasedCurveDataTable;

	// /** 引用 Ability Tag 配置 DataTable */
	UPROPERTY(EditAnywhere, Category = "Config|Curve")
	TObjectPtr<UCurveFloat> PlayerLevelCurveFloat;

	//TODO:即将进行添加
	UPROPERTY(EditAnywhere, Category = "Config|Curve")
	TObjectPtr<UCurveTable> EnemyXPCurveTable;

	/** 引用 CharacterInfo 数据资产 */
	UPROPERTY(EditAnywhere, Category = "Config|Character")
	TObjectPtr<UCharacterInfo> CharacterInfo;

	/** 引用 InputInfo 输入配置数据资产 */
	UPROPERTY(EditAnywhere, Category = "Config|Input")
	TObjectPtr<UInputInfo> InputInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Ability")
	TObjectPtr<UAttributeInfo> AttributeInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Ability")
    TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Attribute")
	TObjectPtr<UWidgetInfo> WidgetInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Inventory")
	TObjectPtr<UInventoryInfo> InventoryInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Input")
	TObjectPtr<UMappableInfo> MappableInfo;

	UPROPERTY(EditAnywhere, Category = "Config|Widget")
	TObjectPtr<UTeamInfo> TeamInfo;
};
